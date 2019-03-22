with open('hi2.txt','r') as f:
    thing = f.readlines()
thing2 = ''.join(thing)
thing3 = thing2.replace('\na 1, b 100, num_samples 100000','')
    
    
data_points = {n:[] for n in range(2,41)}
f = thing3.split('\n')
for line in f:
    data_index, time = line.split(': ')
    data_index = int(data_index)
    time = float(time)
    data_points[data_index].append(time)
    
for num_procs in data_points.keys():
    print("{}: {}".format(num_procs,sum(data_points[num_procs])/len(data_points[num_procs])))