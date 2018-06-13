import sqlite3

from fpdf import FPDF

import barcode
from barcode.writer import ImageWriter

def generate_label(return_id):
    db = sqlite3.connect('mydb')
    cursor = db.cursor()
    cursor.execute('''SELECT customers.name,
            customers.address,
            customers.zipcode,
            customers.city,
            products.name,
            products.size,
            orders.date,
            orders.id,
            customers.id,
            products.id,
            returns.id,
            products_returned.id,
            products_returned.return_reason_id
            FROM products_returned 
            INNER JOIN returns          ON returns.id          = products_returned.return_id 
            INNER JOIN products_ordered ON products_ordered.id = products_returned.product_order_id
            INNER JOIN orders           ON orders.id           = products_ordered.order_id 
            INNER JOIN products         ON products.id         = products_ordered.product_id 
            INNER JOIN customers        ON customers.id        = orders.customer_id
            WHERE returns.id = ?''', (return_id, ))
    data = cursor.fetchall()
    firstrow = data[0]

    cursor.execute('''SELECT id, description 
                      FROM return_reasons''')
    return_reasons = cursor.fetchall()

    db.close()

    pdf = FPDF()
    pdf.add_page()

    # title and logo at top left
    pdf.set_font('Arial', 'B', 16)
    pdf.image('logo.png', 10, 10, 33)
    pdf.set_xy(10, 45)
    pdf.cell(40, 10, 'MyCorp', ln=1, align='l')

    # name and address
    pdf.set_xy(10, 80)
    pdf.cell(40, 10, firstrow[0],  ln=1, align='l')
    pdf.cell(40, 10, firstrow[1],  ln=1, align='l')
    pdf.cell(40, 10, "%s %s" % (firstrow[2], firstrow[3]), ln=1, align='l')

    # bar code
    bclass = barcode.get_barcode_class('code39')
    code = bclass(str(return_id), writer=ImageWriter(), add_checksum=False)
    fullname = code.save('barcode')
    pdf.image('barcode.png', 140, 10, 60)

    # order number and customer number
    pdf.set_xy(140, 80)
    pdf.cell(40, 10, "Order number: %s" % firstrow[7], ln=1, align='l')

    # text with description
    pdf.set_xy(10, 120)
    pdf.cell(40, 10, "Return label", ln=1, align='l')

    # table with article number, description, size, reason
    pdf.set_font('Arial', 'B', 12)
    pdf.cell(40, 10, "Product number",  border=1, ln=0, align='l')
    pdf.cell(80, 10, "Description",     border=1, ln=0, align='l')
    pdf.cell(30, 10, "Size",            border=1, ln=0, align='l')
    pdf.cell(30, 10, "Reason",          border=1, ln=1, align='l')
    num_rows = len(data)
    for row in data:
        pdf.cell(40, 10, str(row[9]),  border=1, ln=0, align='l')
        pdf.cell(80, 10, row[4],       border=1, ln=0, align='l')
        pdf.cell(30, 10, row[5],       border=1, ln=0, align='l')
        pdf.cell(30, 10, str(row[12]), border=1, ln=1, align='l')
    for i in xrange(8 - num_rows):
        pdf.cell(40, 10, '', border=1, ln=0, align='l')
        pdf.cell(80, 10, '', border=1, ln=0, align='l')
        pdf.cell(30, 10, '', border=1, ln=0, align='l')
        pdf.cell(30, 10, '', border=1, ln=1, align='l')

    # reason code descriptions
    pdf.set_font('Arial', 'B', 10)
    pdf.set_xy(10, 230)
    pdf.cell(40,  8, 'Reason code', border=1, ln=0, align='l')
    pdf.cell(100, 8, 'Description', border=1, ln=1, align='l')
    for reason_id, desc in return_reasons:
        pdf.cell(40,  8, str(reason_id), border=1, ln=0, align='l')
        pdf.cell(100, 8, desc,           border=1, ln=1, align='l')

    return pdf

if __name__ == '__main__':
    import sys
    return_id = int(sys.argv[1])
    pdf = generate_label(return_id)
    pdf.output('output.pdf', 'F')
