import redis
r = redis.Redis(host = "localhost", port = 16378)
r.execute_command("AUTH", "asdf")
r.flushdb()
fp = open("cert.pem", "r")
r.set("KeyCenterAdminPublicKey", fp.read())
