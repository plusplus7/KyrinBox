import test_sdk

def main():
    k = test_sdk.TestSdk("Admin", "../deploy/key.pem")
    fp = open("test_cert.pem", "r")
    print k.set_kyrin_key("TestKyrinKey", fp.read())
    fp.close()

if __name__ == "__main__":
    main()
