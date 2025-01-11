import sys, os
BASE_DIR = os.path.dirname(os.path.dirname(__file__)) 
sys.path.append(BASE_DIR) 

import fixture

payloads = [
    {
        "request_id": fixture.generate_random_uint64(),
        "use_case": "calculate_math_expression",
        "payload": {
            "expression": "..."
        }
    },
    {
        "request_id": fixture.generate_random_uint64(),
        "use_case": "calculate_math_expression",
        "payload": {
            "expression": "1+2"
        }
    }
]

@fixture.with_default_host_and_port()
def send_some_tcp_request(host, port, some_payloads):
    return fixture.send_some_tcp_requests(host, port, some_payloads)

send_some_tcp_request(
    some_payloads=payloads)