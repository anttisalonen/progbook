import sqlite3

db = sqlite3.connect('mydb')
cursor = db.cursor()

print 'distribution of return reasons:'
cursor.execute('''SELECT   return_reason_id, count(products_returned.id)
                  FROM     products_returned
                  GROUP BY return_reason_id
                  ORDER BY count(products_returned.id) DESC
                  LIMIT    3''')
for row in cursor.fetchall():
    print row

print 'top 5 ordered products:'
cursor.execute('''SELECT     products.id, products.name, products.size, count(products_ordered.id)
                  FROM       products
                  INNER JOIN products_ordered ON products_ordered.product_id = products.id
                  GROUP BY   products.id
                  ORDER BY   count(products_ordered.id) DESC
                  LIMIT      5''')
for row in cursor.fetchall():
    print row

print 'top ordering customers:'
cursor.execute('''SELECT     customers.id, customers.name, count(orders.id)
                  FROM       customers
                  INNER JOIN orders ON orders.customer_id = customers.id
                  GROUP BY   customers.id
                  ORDER BY   count(orders.id) DESC
                  LIMIT      10''')
for row in cursor.fetchall():
    print row

print 'product 123:'
cursor.execute('''SELECT     products.id, products.name, products.size
                  FROM       products
                  WHERE products.id = 123''')
for row in cursor.fetchall():
    print row

print 'products ordered by customer 123:'
cursor.execute('''SELECT     customers.id, customers.name, products.id, products.name, products.size
                  FROM       customers
                  INNER JOIN orders           ON orders.customer_id          = customers.id
                  INNER JOIN products_ordered ON products_ordered.order_id   = orders.id
                  INNER JOIN products         ON products_ordered.product_id = products.id
                  WHERE customers.id = ?''', (123, ))
for row in cursor.fetchall():
    print row

quantifiers = [('most', 'DESC'), ('least', 'ASC')]
for q1, q2 in quantifiers:
    print 'who has returned %s products:' % q1
    cursor.execute('''SELECT     customers.id, customers.name, customers.address, count(products_returned.id)
                      FROM       products_returned
                      INNER JOIN returns   ON returns.id   = products_returned.return_id
                      INNER JOIN orders    ON orders.id    = returns.order_id
                      INNER JOIN customers ON customers.id = orders.customer_id
                      GROUP BY   customers.id
                      ORDER BY   count(products_returned.id) %s
                      LIMIT      5''' % q2)
    for row in cursor.fetchall():
        print row

print 'who has ordered most products:'
cursor.execute('''SELECT     customers.id, customers.name, customers.address, count(products_ordered.id)
                  FROM       products_ordered
                  INNER JOIN orders           ON orders.id           = products_ordered.order_id
                  INNER JOIN customers        ON customers.id        = orders.customer_id
                  GROUP BY   customers.id
                  ORDER BY   count(products_ordered.id) DESC
                  LIMIT      5''')
for row in cursor.fetchall():
    print row

print 'highest return to ordered product ratio:'
cursor.execute('''SELECT     customers.id, customers.name, customers.address, count(distinct products_ordered.id), count(distinct products_returned.id), (cast(count(distinct products_returned.id) AS FLOAT) / count(distinct products_ordered.id)) AS ratio
                  FROM       customers
                  INNER JOIN orders            ON orders.customer_id          = customers.id
                  INNER JOIN products_ordered  ON products_ordered.order_id   = orders.id
                  INNER JOIN returns           ON returns.order_id            = orders.id
                  INNER JOIN products_returned ON products_returned.return_id = returns.id
                  GROUP BY   customers.id
                  ORDER BY   ratio DESC
                  LIMIT      5''')
for row in cursor.fetchall():
    print row

quantifiers = [('most', 'DESC'), ('least', 'ASC')]
for q1, q2 in quantifiers:
    print '%s returned products:' % q1
    cursor.execute('''SELECT     products.id, products.name, products.size, count(products_returned.id)
                      FROM       products_returned
                      INNER JOIN products_ordered ON products_ordered.id = products_returned.product_order_id
                      INNER JOIN products         ON products.id         = products_ordered.product_id
                      GROUP BY   products.id
                      ORDER BY   count(products_returned.id) %s
                      LIMIT      5''' % q2)
    for row in cursor.fetchall():
        print row

db.close()
