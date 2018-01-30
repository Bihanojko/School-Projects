# Subject: Computer Communications and Networks
# Project: Project 2 - File transfer
# Author: Nikola Valesova
# Login: xvales02
# Date: 24. 4. 2016
# Test.sh

# compile server and client
make

echo "running server with port number 55000"
./server -p 55000 &

echo "TEST 1: upload"
./client -p 55000 -h localhost -u test0.txt

echo "checking if correct"
diff test00.txt test0.txt

echo "TEST 2: download"
./client -p 55000 -h localhost -d test1.txt

echo "checking if correct"
diff test01.txt test1.txt

echo "TEST 3: upload from a different directory"
./client -p 55000 -h localhost -d ./txt/test2.txt

echo "checking if correct"
diff test02.txt ./txt/test2.txt

echo "TEST 4: trying to connect to an invalid port number -> ERROR"
./client -p 555000 -h localhost -d test.txt

echo "TEST 5: uploading a non-existent file -> ERROR"
./client -p 55000 -h localhost -u my_file.txt

echo "closing server"
kill -9 `ps | grep "server" | awk '{ print $1 }'`

echo "removing files created by make"
make clean
