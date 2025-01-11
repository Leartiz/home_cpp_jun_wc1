# Protocol over TCP 🧬

## Packet

### Header (TCP)

- Длина тела. 4 байта, uint32, big endian.

### Body (with internal header?). Request, response

- Строка в формате JSON.

#### Examples

```hex
00 00 00 4A // заголовок (дальше без разделителя)
7B 0A 20 20 20 20 22 73 65 72 76 ... // тело
```

<!-- -------------------------------------------- -->

## Use Case 🧭

## `Calculate math expression`

### Request ->

```json
{
    "request_id": "<uint64>",
    "use_case": "calculate_math_expression",

    "payload": {
        "expression": "<string>"
    }
}
```

#### Examples

```json
{
    "request_id": 12345678901234567890,
    "use_case": "calculate_math_expression", // !

    "payload": {
        "expression": "3 + 5 * (2 - 8)"
    }
}
```

### Response <-

```json
{
    "request_id": "<uint64>",
    "status_code": "<HTTP status code as int>",

    // or
    "result": {
        "value": "<float64>"
    },
    "error": {
        "message": "<string>"
    },

    // always
    "metadata": {
        "timestamp": "<timestamp as string>",
        "duration": "<duration as string>"
    }
}
```

#### Examples

##### OK ✅

```json
{
    "request_id": 12345678901234567890,
    "status_code": 200,

    "result": {
        "value": 7.0
    },
    "error": null,

    "metadata": {
        "timestamp": "2024-12-15T15:30:01Z",
        "duration": "35ms"
    }
}
```

##### Error ❌

```json
{
    "request_id": 12345678901234567890,
    "status_code": 400,

    "result": null,
    "error": {
        "message": "expression/result is not a number"
    },

    "metadata": {
        "timestamp": "2024-12-15T15:30:01Z",
        "duration": "5ms"
    }
}
```

<!-- -------------------------------------------- -->