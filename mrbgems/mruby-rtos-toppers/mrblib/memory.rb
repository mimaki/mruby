module RTOS
  class MemoryPool
    # mempool#get(tmo=:forever)  # => MemoryBuffer
    #
    # Get MemoryBuffer from MemoryPool.
    #
    # Parameters:
    #   +tmo+     Wait time for getting buffer
    #     :polling  No wait
    #     :forever  Wait forever
    #     other     Wait time (ms)
    #
    # Returns MemoryBuffer object.
    def get(tmo=:forever)
      MemoryBuffer.new(@id, @blksz, tmo)
    end

    # mempool#release(mb)  # => nil
    #
    # Release MemoryBuffer to MemoryPool.
    #
    # Parameters:
    #   +mb+      MessageBuffer
    #
    # Returns nil.
    def release(mb)
      mb.release
    end
  end
end
