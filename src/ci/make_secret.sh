openssl genrsa -out secret/key.pem 2048
openssl rsa -in secret/key.pem -pubout -out secret/cert.pem
