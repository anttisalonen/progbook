#!/bin/bash

set -e

#echo Creating database
#python2 ins.py
echo Testing queries
python2 select.py
echo Generating pdf
python2 print_return.py 12345
echo Testing web shop

set +e
FLASK_APP=shop.py flask run --port 8554 &
FLASK_PID=$?
sleep 2

rm -rf testing
mkdir testing
pushd testing
curl -s "http://127.0.0.1:8554/products/" > products.html
curl -s "http://127.0.0.1:8554/orders/?customer_id=5" > orders.html
curl -s "http://127.0.0.1:8554/order/?order_id=148" > order.html
curl -s "http://127.0.0.1:8554/return.html?order_id=148&109=on&174=on" > ret.html
curl -s -d "order_id=148&109=1&174=4" "http://127.0.0.1:8554/print.html" > out.pdf

kill $FLASK_PID
popd
