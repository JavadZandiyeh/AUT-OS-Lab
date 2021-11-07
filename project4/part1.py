from multiprocessing import shared_memory
import array

# create shared memory with size 20
shm_a = shared_memory.SharedMemory(create=True, size=20)

# put something in buffer of shared memory
buffer = shm_a.buf
buffer[:] = bytearray([i for i in range(20)])

# attach to an existing shared memory block
shm_b = shared_memory.SharedMemory(shm_a.name)

# copy data of buffer into an array and print it
data = array.array('d', shm_b.buf[:])
print(data[2:8])

# close access to shared memory from instances b and a
shm_b.close()
shm_a.close()

# destroy shared memory block
shm_a.unlink()
