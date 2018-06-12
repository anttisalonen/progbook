import random

import sqlite3

random.seed(21)

db = sqlite3.connect('mydb')
cursor = db.cursor()
for table in ['customers', 'products', 'orders',
        'products_ordered', 'returns', 'products_returned',
        'return_reasons']:
    cursor.execute('DROP TABLE if exists %s' % table)
    db.commit()

cursor.execute('''
        CREATE TABLE customers(id INTEGER PRIMARY KEY,
                               name TEXT,
                               address TEXT,
                               zipcode TEXT,
                               city TEXT)''')
cursor.execute('''
        CREATE TABLE products(id INTEGER PRIMARY KEY,
                               name TEXT,
                               size TEXT)''')
cursor.execute('''
        CREATE TABLE orders(id INTEGER PRIMARY KEY,
                               date DATE,
                               customer_id INTEGER,
                               FOREIGN KEY(customer_id) REFERENCES customers(id))''')
cursor.execute('''
        CREATE TABLE products_ordered(id INTEGER PRIMARY KEY,
                               order_id INTEGER,
                               product_id INTEGER,
                               FOREIGN KEY(order_id) REFERENCES orders(id),
                               FOREIGN KEY(product_id) REFERENCES products(id))''')
cursor.execute('''
        CREATE TABLE returns(id INTEGER PRIMARY KEY,
                               order_id INTEGER UNIQUE,
                               FOREIGN KEY(order_id) REFERENCES orders(id))''')
cursor.execute('''
        CREATE TABLE return_reasons(id INTEGER PRIMARY KEY,
                               description TEXT)''')
for description in ['Too large', 'Too small', 'Not as expected', 'Other']:
    cursor.execute('INSERT INTO return_reasons(description) VALUES(?)',
            (description, ))
cursor.execute('''
        CREATE TABLE products_returned(id INTEGER PRIMARY KEY,
                               return_id INTEGER,
                               return_reason_id INTEGER,
                               product_order_id INTEGER,
                               FOREIGN KEY(product_order_id) REFERENCES products_ordered(id),
                               FOREIGN KEY(return_id) REFERENCES returns(id),
                               FOREIGN KEY(return_reason_id) REFERENCES return_reasons(id))''')
db.commit()

for i in xrange(100):
    color = random.choice(['Red', 'Green', 'Blue', 'Yellow', 'Black', 'White', 'Orange'])
    gender = random.choice(["Men's", "Women's"])
    clothing = random.choice(['Jeans', 'Shirt', 'T-Shirt', 'Cardigan', 'Skirt', 'Dress', 'Belt'])
    name = '%s %s %s' % (color, gender, clothing)
    sizes = random.choice([['S', 'M', 'L', 'XL'], ['36', '38', '40', '42']])
    for size in sizes:
        cursor.execute('INSERT INTO products(name, size) VALUES(?, ?)',
                (name, size))

cursor.execute('SELECT id FROM products')
all_product_ids = cursor.fetchall()
all_product_ids = [x for (x,) in all_product_ids]

cursor.execute('SELECT count(*) from return_reasons')
num_return_reasons = cursor.fetchone()[0]
assert num_return_reasons > 0

for i in xrange(1000):
    first_name = random.choice(['John', 'Jane', 'Roger', 'Maria', 'Andy', 'William', 'Leah'])
    surname = random.choice(['Smith', 'Williams', 'Baker', 'Armstrong', 'Miller'])
    name = '%s %s' % (first_name, surname)
    housenr = random.randint(1, 8000)
    street_name = random.choice(['Elm', 'Oak',
        'Maple', 'Penn', 'Pine', 'Ash', 'Aspen',
        'Cherry', 'Willow', 'Beech'])
    road = random.choice(['Road', 'Street', 'Avenue', 'Lane', 'Parkway'])
    zipcode = random.randint(10000, 99999)
    city = random.choice(['New York', 'Washington D.C.', 'Chicago'])
    address = '%s %s %s' % (housenr, street_name, road)

    cursor.execute('INSERT INTO customers(name, address, zipcode, city) VALUES(?, ?, ?, ?)',
            (name, address, zipcode, city))
    customer_id = cursor.lastrowid

    # each customer has between 2 and 50 orders
    for i in xrange(random.randint(2, 50)):
        year = random.choice([2015, 2016, 2017, 2018])
        month = random.randint(1, 12)
        day = random.randint(1, 28)
        date = "%04d-%02d-%02d" % (year, month, day)
        cursor.execute('INSERT INTO orders(date, customer_id) VALUES(?, ?)',
                (date, customer_id))
        order_id = cursor.lastrowid
        product_order_ids = list()
        num_products_ordered = random.randint(2, 7)

        # each order has between 2 and 7 products
        for j in xrange(num_products_ordered):
            product_id = random.choice(all_product_ids)
            cursor.execute('INSERT INTO products_ordered(order_id, product_id) VALUES(?, ?)',
                    (order_id, product_id))
            product_order_ids.append(cursor.lastrowid)
        random.shuffle(product_order_ids)

        # each order may have some returns
        return_id = None
        for j in xrange(random.randint(0, num_products_ordered)):
            if not return_id:
                cursor.execute('INSERT INTO returns(order_id) VALUES (?)',
                        (order_id, ))
                return_id = cursor.lastrowid
            product_order_id = product_order_ids.pop()
            return_reason_id = random.randint(1, num_return_reasons)
            cursor.execute('''INSERT INTO products_returned(
                        return_id, return_reason_id, product_order_id) VALUES (?, ?, ?)''',
                        (return_id, return_reason_id, product_order_id))

db.commit()
db.close()

