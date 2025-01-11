import sys, os
BASE_DIR = os.path.dirname(os.path.dirname(__file__)) 
sys.path.append(BASE_DIR) 

import fixture

data_to_send = {
    "request_id": 12345,
    "use_case": "calculate_math_expression",
    "payload": {
        "expression": "1 + 2 + 3 + 4 + 5"
    }
}

@fixture.with_default_host_and_port()
def send_tcp_request(host, port, data_to_send):
    return fixture.send_tcp_request(host, port, data_to_send)

send_tcp_request(
    data_to_send=data_to_send)