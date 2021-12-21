""" 

sub-classed ring buffer with the following changes:
1. writer may overwrite data input does not hang if readers cannot keep up
2. "reader" pointer is ignored, always reads the n-most recent frames.

"""

import ctypes
from cringbuffer.ringbuffer import RingBuffer, AlreadyClosedError, Pointer, WaitingForWriterError
from typing import Type

IMG_CHANNELS = 3
IMG_WIDTH = 640
IMG_HEIGHT = 480

class Frame(ctypes.Structure):
    _fields_ = [
        ("timestamp_us", ctypes.c_ulonglong),
        ("frame", ctypes.c_ubyte * IMG_CHANNELS * IMG_WIDTH * IMG_HEIGHT)
    ]
    
class OverwritingRingBuffer(RingBuffer):
    
    
    def __init__(self, *, c_type: Type[ctypes._SimpleCData], slot_count: int):
        super().__init__(c_type, slot_count)

        
    def write(self, data: ctypes._SimpleCData) -> None:
        """writes the next slot, but will not block.

        Once a successful write occurs, all pending blocking_read() calls
        will be woken up to consume the newly written slot.

        Args:
            data: Object to write in the next available slot.

        Raises:
            WaitingForReaderError: If all of the slots are full and we need
                to wait for readers to catch up before there will be
                sufficient room to write more data. This is a sign that
                the readers can't keep up with the writer. Consider calling
                force_reader_sync() if you need to force the readers to
                catch up, but beware that means they will miss data.
        """
        with self.lock.for_write():
            if not self.active.value:
                raise AlreadyClosedError

            position = self.writer.get()
            # ignore write conflicts
            # if self._has_write_conflict(position):
            #     raise WaitingForReaderError

            self.array[position.index] = data
            self.writer.increment()


    def _try_read_no_lock(self, reader: Pointer,
                          length: int = 1) -> ctypes._SimpleCData:
        """Overridden function to read n-most recent frames
        
        This function is called by `try_read` which handles the lock
        """        
        writer_position = self.writer.get()
        
        # We want to read the last n frames, regardless if they have been previously read 
        # using the writer's current position find my position
        position = writer_position
        position.counter = writer_position.counter - length

        # if position counter is negative then there is not enough data recorded yet
        # wait for writer
        if position.counter < 0:
            raise WaitingForWriterError

        new_array = (self.c_type * length)()

        remaining = self.slot_count - position.index
        ctypes.memmove(new_array,
                       ctypes.addressof(self.array[position.index]),
                       self.slot_size * min(length, remaining))
        if length > remaining:
            ctypes.memmove(
                ctypes.addressof(new_array[remaining]),
                ctypes.addressof(self.array),
                (length - remaining) * self.slot_size)

        # reader.increment_by(length)
        return new_array