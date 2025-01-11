import collections

orders = collections.defaultdict(list)

orders[1].append(1)
orders[1].append(2)
orders[1].append(3)
orders[1].append(2)

orders[2].append(6)
orders[2].append(5)

print(orders)
most_recent_key = list(orders)[-1]
print(orders[most_recent_key][0]) # err!