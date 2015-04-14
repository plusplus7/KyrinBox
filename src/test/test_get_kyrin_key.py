import test_sdk

def main():
    k = test_sdk.TestSdk("Admin", "../deploy/key.pem")
    print k.get_kyrin_key("TestKyrinKey")

if __name__ == "__main__":
    main()
