from flask import Flask, render_template, request, make_response
app = Flask(__name__)

import sqlite3
import print_return

db = sqlite3.connect('mydb')
cursor = db.cursor()

def make_product(p):
    if len(p) > 3:
        return {'id': p[0], 'name': p[1], 'size': p[2], 'product_order_id': p[3]}
    else:
        return {'id': p[0], 'name': p[1], 'size': p[2]}

def make_order(p):
    return {'id': p[0], 'date': p[1], 'customer_id': p[2]}

def make_reason(p):
    return {'id': p[0], 'description': p[1]}

def get_order_products(order_id):
    cursor.execute('''SELECT     products.id, products.name, products.size, products_ordered.id
                      FROM       products
                      INNER JOIN products_ordered ON products_ordered.product_id = products.id
                      INNER JOIN orders           ON orders.id                   = products_ordered.order_id
                      WHERE      orders.id = ?''', (order_id, ))
    product_list = cursor.fetchall()
    return [make_product(o) for o in product_list]

@app.route("/products/", methods=['GET'])
def products():
    cursor.execute('''SELECT   * FROM products''')
    product_list = cursor.fetchall()
    product_list = [make_product(p) for p in product_list]
    return render_template('products.html', product_list=product_list)

@app.route("/orders/", methods=['GET'])
def orders():
    customer_id = request.args.get('customer_id', 1)
    cursor.execute('''SELECT     *
                      FROM       orders
                      INNER JOIN customers ON customers.id = orders.customer_id
                      WHERE      customers.id = ?''', (customer_id, ))
    order_list = cursor.fetchall()
    order_list = [make_order(o) for o in order_list]
    return render_template('orders.html', order_list=order_list, customer_id=customer_id)

@app.route("/order/", methods=['GET'])
def order():
    order_id = request.args.get('order_id', 1)
    product_list = get_order_products(order_id)
    return render_template('order.html', product_list=product_list, order_id=order_id)

@app.route("/return.html", methods=['GET'])
def return_get_reason():
    order_id = request.args.get('order_id')
    product_list = get_order_products(order_id)
    product_list = [p for p in product_list if request.args.get(str(p['id']))]
    cursor.execute('''SELECT * FROM return_reasons''')
    reason_list = cursor.fetchall()
    reason_list = [make_reason(o) for o in reason_list]
    reason_list[0]['default'] = True
    return render_template('return.html', product_list=product_list, reason_list=reason_list, order_id=order_id)

@app.route("/print.html", methods=['POST'])
def return_print():
    # build database input
    order_id = request.form['order_id']
    product_list = get_order_products(order_id)
    product_list = [p for p in product_list if request.form.get(str(p['id']))]
    reasons = list()
    for p in product_list:
        reason = request.form.get(str(p['id']), 4)
        reasons.append(reason)
    product_list = zip(product_list, reasons)

    # update database
    cursor.execute('INSERT OR IGNORE INTO returns(order_id) VALUES (?)',
            (order_id, ))
    cursor.execute('SELECT id FROM returns WHERE order_id = ?', (order_id, ))
    return_id = cursor.fetchone()[0]
    cursor.execute('DELETE FROM products_returned WHERE return_id = ?', (return_id, ))
    for p, r in product_list:
        product_order_id = p['product_order_id']
        cursor.execute('''INSERT INTO products_returned(
                    return_id, return_reason_id, product_order_id) VALUES (?, ?, ?)''',
                    (return_id, r, product_order_id))
    db.commit()

    # generate and send pdf
    pdf = print_return.generate_label(return_id)
    binary_stream = pdf.output('', 'S')
    response = make_response(binary_stream)
    response.headers['Content-Type'] = 'application/pdf'
    response.headers['Content-Disposition'] = 'inline; filename=return.pdf'
    return response

