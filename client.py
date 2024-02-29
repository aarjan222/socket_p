import socket
import struct
import time

"""
PORT NUMBER AND IP ADDRESS SHOULD BE SAME FOR BOTH SERVER AND CLIENT!!!!!
"""
PORT = 3333
SERVER_IP = "127.0.0.1"


def main():
    # 1. CREATE THE SOCKET
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 2. CONNECT TO THE SERVER
    client_socket.connect((SERVER_IP, PORT))

    # DATA TO SEND TO SERVER
    client_data = [3.0213, 12.032, 678.0567]

    while (1):
        # BUFFER FOR SENDING DATA
        buffer_data = bytearray()

        # PACK DATA INTO BUFFER
        for value in client_data:
            buffer_data.extend(struct.pack('f', value))

        # SEND DATA FROM BUFFER TO THE SERVER
        client_socket.sendall(buffer_data)

        # RECIEVE DATA FROM SERVER
        receive_buffer = client_socket.recv(12)

        # UNPACK RECEIVED DATA
        server_data = struct.unpack('fff', receive_buffer)

        print("Received data from server:")
        for i, value in enumerate(server_data):
            print(f"server_data[{i}] = {value:.2f}")

        # CHANGE DELAY VALUES TO ENSURE
        #         1. DATA ISNOT MISPLACED.
        #         2. DATA RECEIVED IN CORRECT ORDER.
        time.sleep(2)

    # CLOSE THE CONNECTION
    client_socket.close()


if __name__ == "__main__":
    main()
