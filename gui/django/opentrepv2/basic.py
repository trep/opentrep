from flask import Flask
app = Flask("search")

@app.route("/v1")
def hello():
    
    return "Hello, I love Digital Ocean!"

if __name__ == "__main__":
    app.run()
