#include "pch.h"
#include "bufferlock.h"

GLuint64 kOneSecondInNanoSeconds = 1000000000;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
BufferLockManager::BufferLockManager(bool _cpuUpdates)
: mCPUUpdates(_cpuUpdates)
{

}

// --------------------------------------------------------------------------------------------------------------------
BufferLockManager::~BufferLockManager()
{
    for (auto it = mBufferLocks.begin(); it != mBufferLocks.end(); ++it) {
        cleanup(&*it);
    }

    mBufferLocks.clear();
}

// --------------------------------------------------------------------------------------------------------------------
void BufferLockManager::WaitForLockedRange(size_t _lockBeginBytes, size_t _lockLength)
{
    BufferRange testRange = { _lockBeginBytes, _lockLength };
    std::vector<BufferLock> swapLocks;
    for (auto it = mBufferLocks.begin(); it != mBufferLocks.end(); ++it)
    {
        if (testRange.Overlaps(it->mRange)) {
            wait(&it->mSyncObj);
            cleanup(&*it);
        } else {
            swapLocks.push_back(*it);
        }
    }

    mBufferLocks.swap(swapLocks);
}

// --------------------------------------------------------------------------------------------------------------------
void BufferLockManager::LockRange(size_t _lockBeginBytes, size_t _lockLength)
{
    BufferRange newRange = { _lockBeginBytes, _lockLength };
    GLsync syncName = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    BufferLock newLock = { newRange, syncName };

    mBufferLocks.push_back(newLock);
}

// --------------------------------------------------------------------------------------------------------------------
void BufferLockManager::wait(GLsync* _syncObj)
{
    if  (mCPUUpdates) {
        GLbitfield waitFlags = 0;
        GLuint64 waitDuration = 0;
        while (1) {
            GLenum waitRet = glClientWaitSync(*_syncObj, waitFlags, waitDuration);
            if (waitRet == GL_ALREADY_SIGNALED || waitRet == GL_CONDITION_SATISFIED) {
                return;
            }

            if (waitRet == GL_WAIT_FAILED) {
                assert(!"Not sure what to do here. Probably raise an exception or something.");
                return;
            }

            // After the first time, need to start flushing, and wait for a looong time.
            waitFlags = GL_SYNC_FLUSH_COMMANDS_BIT;
            waitDuration = kOneSecondInNanoSeconds;
        }
    } else {
        glWaitSync(*_syncObj, 0, GL_TIMEOUT_IGNORED);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void BufferLockManager::cleanup(BufferLock* _bufferLock)
{
    glDeleteSync(_bufferLock->mSyncObj);
}
