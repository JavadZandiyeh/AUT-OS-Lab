import psutil
from tabulate import tabulate

print("\n*** Iterate over all running process and print process ID & Name ***")
# Iterate over all running process
process_table = []
for proc in psutil.process_iter():
    try:
        processName = proc.name()
        processID = proc.pid
        processThreads = proc.num_threads()
        processMemoryUsage = proc.memory_info().vms / (1024 * 1024)
        process_table.append([processID, processName, processThreads, processMemoryUsage])
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
        pass
print(tabulate(process_table, headers=['pid', 'name', 'num_threads', 'memory_usage']))

print('\n*** Top 10 process with highest memory usage ***')    
sorted_process_table = sorted(process_table, key=lambda procObj: procObj[3], reverse=True)
top5 = sorted_process_table[0:10]
print(tabulate(top5, headers=['pid', 'name', 'num_threads', 'memory_usage']))
