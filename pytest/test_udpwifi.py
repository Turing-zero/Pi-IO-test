import socket

SERVER_IP = "192.168.31.207"
SERVER_PORT = 5005
BUFFER_SIZE = 1024

# sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
# # 连接到一个外部地址（这里是Google的DNS服务器）
# sock.connect(("8.8.8.8", 80))
# # 获取本地wifiIP地址
# ip = sock.getsockname()[0]
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind((SERVER_IP,SERVER_PORT))

def send_data(data,ip,port):
    # message = "Hello from Rasberry Pi"
    sock.sendto(data.encode(),(ip,port))

def recv_data():
    data,addr = sock.recvfrom(BUFFER_SIZE)
    return data,addr

while True:
    data,addr = recv_data()
    send_data("Hello",addr[0],addr[1])
    # print(data)