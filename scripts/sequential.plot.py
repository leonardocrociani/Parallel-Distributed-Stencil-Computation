import matplotlib.pyplot as plt

with open('./data/results/sequential.csv', 'r') as f:
    data = f.read()
    
data = data.split('\n')[1:-1]
data = [d.split(',') for d in data]
data = [(int(d[0]), float(d[1])) for d in data]

x = [d[0] for d in data]
y = [d[1] for d in data]

plt.figure(figsize=(10, 10))
plt.plot(x, y, marker='o', label='Time')
plt.title('Time of exection of the sequential version')
plt.xlabel('Matrix size')
plt.ylabel('Time (ms)')
plt.title('Sequential execution time')
        
plt.grid()
plt.tight_layout()
plt.savefig('./data/plots/sequential/sequential.png')
plt.close()