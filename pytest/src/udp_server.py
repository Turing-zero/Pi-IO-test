import socket 

UDP_IP = '192.168.31.207'
UDP_PORT = 5005

udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
udp_socket.bind((UDP_IP,UDP_PORT))

# def recv_udp_data():
#     while True:
#         data,addr=udp_socket.recvfrom(1024)
#         return data
# def send_udp_data(data):

while True:
    data,addr=udp_socket.recvfrom(1024)
    print("Received message:",data.decode(),"from",addr)

    response = "Message received"
    udp_socket.sendto(response.encode(), addr)
    
    # 发送额外的消息到客户端
    extra_message = "Hello from Raspberry Pi"
    udp_socket.sendto(extra_message.encode(), addr)    
    
