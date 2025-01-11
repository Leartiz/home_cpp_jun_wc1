import socket, time
import json, struct

import secrets
from functools import wraps

# ------------------------------------------------------------------------



def generate_random_uint64():
    return secrets.randbits(64)

def send_some_tcp_requests(host, port, some_data: list):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((host, port))
            
            for data in some_data:
                json_data = json.dumps(data)
                json_bytes = json_data.encode('utf-8')
                
                data_length = len(json_bytes)
                header = struct.pack('!I', data_length)

                packet = header + json_bytes

                sock.sendall(packet)
                
                response = sock.recv(1024)  
                print(f"Received response: {response}")
            
            time.sleep(0.1)
            
    except Exception as e:
        print(f"An error occurred: {e}")


def send_tcp_request(host, port, data):
    send_some_tcp_requests(host, port, [data])
     
# ------------------------------------------------------------------------        
        
def with_default_host_and_port(default_host="127.0.0.1", 
                               default_port=24444):
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            _ = args
            
            host = kwargs.get("host", default_host)
            port = kwargs.get("port", default_port)
            some_payloads = kwargs.get("some_payloads")
            
            return func(host, port, some_payloads)
        return wrapper
    return decorator