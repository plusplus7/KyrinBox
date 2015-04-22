import redis
r = redis.Redis(host = "localhost", port = 16378)
r.execute_command("AUTH", "asdf")
r.flushdb()
fp = open("secret/cert.pem", "r")
public_key = fp.read()
r.set("KeyCenterAdminPublicKey", public_key)
r.set("plusplus7", public_key)
