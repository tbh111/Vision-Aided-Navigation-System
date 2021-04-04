import socket
import struct
import cv2
import numpy as np
from VGG import VGGMatcher
matcher = VGGMatcher()
server = socket.socket(type=socket.SOCK_DGRAM)
server.bind(('127.0.0.1', 9999))
print("waiting...")
while True:

    data, addr = server.recvfrom(256)  # shoud be b'I BEGIN'
    print(str(data) + " from " + str(addr))
    data, addr = server.recvfrom(16)
    # print(data)
    # img_size = struct.unpack(">L", data)[0]
    img_size = int.from_bytes(data, byteorder='little')  # img size, (bytes)
    # print(img_size)

    img_recv_all = b''
    recvd_size = 0
    # '''
    while True:
        data, addr = server.recvfrom(4096)  # 阻塞等待数据(value,(ip,port))
        # print(img_file)

        img_recv_all += data
        recvd_size += len(data)

        if recvd_size >= img_size:
            break
    # '''

    img_nparr = np.fromstring(img_recv_all, np.uint8)
    img_decoded = cv2.imdecode(img_nparr, cv2.IMREAD_COLOR)
    cv2.imwrite("123.jpg", img_decoded)
    matcher.match_image("123.jpg")
    # cv2.imshow("WHAT YOU GET", img_decoded)
    # cv2.waitKey(10)


# cv2.destroyAllWindows()
