#pragma once

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
struct BufferRange
{
    size_t mStartOffset;
    size_t mLength;

    bool Overlaps(const BufferRange& _rhs) const {
        return mStartOffset < (_rhs.mStartOffset + _rhs.mLength)
            && _rhs.mStartOffset < (mStartOffset + mLength);
    }
};

// --------------------------------------------------------------------------------------------------------------------
struct BufferLock
{
    BufferRange mRange;
    GLsync mSyncObj;
};

// --------------------------------------------------------------------------------------------------------------------
class BufferLockManager
{
public:
    BufferLockManager(bool _cpuUpdates);
    ~BufferLockManager();

    void WaitForLockedRange(size_t _lockBeginBytes, size_t _lockLength);
    void LockRange(size_t _lockBeginBytes, size_t _lockLength);

private:
    void wait(GLsync* _syncObj);
    void cleanup(BufferLock* _bufferLock);

    std::vector<BufferLock> mBufferLocks;

    // Whether it's the CPU (true) that updates, or the GPU (false)
    bool mCPUUpdates;
};
