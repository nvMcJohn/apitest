#pragma once

#include "bufferlock.h"
#include "console.h"

// --------------------------------------------------------------------------------------------------------------------
enum class BufferStorage 
{
    SystemMemory,
    PersistentlyMappedBuffer
};

// --------------------------------------------------------------------------------------------------------------------
template <typename Atom>
class Buffer
{
public:
    Buffer(bool _cpuUpdates)
    : mLockManager(_cpuUpdates)
    , mBufferContents()
    , mName()
    , mTarget()
    , mBufferStorage(BufferStorage::SystemMemory)
    { }

    ~Buffer()
    {
        Destroy();
    }

    bool Create(BufferStorage _storage, GLenum _target, GLuint _atomCount, GLbitfield _createFlags, GLbitfield _mapFlags)
    {
        if (mBufferContents) {
            Destroy();        
        }

        mBufferStorage = _storage;
        mTarget = _target;

        switch (mBufferStorage) {
            case BufferStorage::SystemMemory: {
                mBufferContents = new Atom[_atomCount];                
                break;
            }

            case BufferStorage::PersistentlyMappedBuffer: {
                // This code currently doesn't care about the alignment of the returned memory. This could potentially
                // cause a crash, but since implementations are likely to return us memory that is at lest aligned
                // on a 64-byte boundary we're okay with this for now. 
                // A robust implementation would ensure that the memory returned had enough slop that it could deal
                // with it's own alignment issues, at least. That's more work than I want to do right this second.

                glGenBuffers(1, &mName);
                glBindBuffer(mTarget, mName);
                glBufferStorage(mTarget, sizeof(Atom) * _atomCount, nullptr, _createFlags);
                mBufferContents = reinterpret_cast<Atom*>(glMapBufferRange(mTarget, 0, sizeof(Atom) * _atomCount, _mapFlags));
                if (!mBufferContents) {
                    console::warn("glMapBufferRange failed, probable bug.");
                    return false;
                }
                break;
            }

            default: {
                console::error("Error, need to update Buffer::Create with logic for mBufferStorage = %d", mBufferStorage);
                break;
            }
        };

        return true;
    }

    // Called by dtor, must be non-virtual.
    void Destroy() 
    {
        switch (mBufferStorage) {
            case BufferStorage::SystemMemory: {
                SafeDeleteArray(mBufferContents);
                break;
            }

            case BufferStorage::PersistentlyMappedBuffer: {
                glBindBuffer(mTarget, mName);
                glUnmapBuffer(mTarget);
                glDeleteBuffers(1, &mName);

                mBufferContents = nullptr;
                mName = 0;
                break;
            }

            default: {
                console::error("Error, need to update Buffer::Create with logic for mBufferStorage = %d", mBufferStorage);
                break;
            }
        };
    }

    void WaitForLockedRange(size_t _lockBegin, size_t _lockLength) { mLockManager.WaitForLockedRange(_lockBegin, _lockLength); }
    Atom* GetContents() { return mBufferContents; }
    void LockRange(size_t _lockBegin, size_t _lockLength) { mLockManager.LockRange(_lockBegin, _lockLength); }

    void BindBuffer() { glBindBuffer(mTarget, mName); }
    void BindBufferBase(GLuint _index) { glBindBufferBase(mTarget, _index, mName); }

private:
    BufferLockManager mLockManager;
    Atom* mBufferContents;
    GLuint mName;
    GLenum mTarget;

    BufferStorage mBufferStorage;
};

// --------------------------------------------------------------------------------------------------------------------
template <typename Atom>
class CircularBuffer
{
public:
    CircularBuffer(bool _cpuUpdates = true)
    : mBuffer(_cpuUpdates)
    { }
    
    bool Create(BufferStorage _storage, GLenum _target, GLuint _atomCount, GLbitfield _createFlags, GLbitfield _mapFlags)
    {
        mSizeAtoms = _atomCount;
        mHead = 0;

        return mBuffer.Create(_storage, _target, _atomCount, _createFlags, _mapFlags);
    }

    void Destroy() 
    {
        mBuffer.Destroy();

        mSizeAtoms = 0;
        mHead = 0;
    }
        
    Atom* Reserve(GLsizeiptr _atomCount)
    {
        if (_atomCount > mSizeAtoms) {
            console::error("Requested an update of size %d for a buffer of size %d atoms.", _atomCount, mSizeAtoms);
        }

        GLsizeiptr lockStart = mHead;

        if (lockStart + _atomCount > mSizeAtoms) {
            // Need to wrap here.
            lockStart = 0;
        }

        mBuffer.WaitForLockedRange(lockStart, _atomCount);
        return &mBuffer.GetContents()[lockStart];
    }

    void OnUsageComplete(GLsizeiptr _atomCount)
    {
        mBuffer.LockRange(mHead, _atomCount);
        mHead = (mHead + _atomCount) % mSizeAtoms;
    }

    void BindBuffer() { mBuffer.BindBuffer(); }
    void BindBufferBase(GLuint _index) { mBuffer.BindBufferBase(_index); }

    GLsizeiptr GetSize() const { return mSizeAtoms; }

private:
    Buffer<Atom> mBuffer;

    GLsizeiptr mHead;
    // TODO: Maybe this should be in the Buffer class?
    GLsizeiptr mSizeAtoms;
};
