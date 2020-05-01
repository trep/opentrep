from flask import Flask
from flask_restful import Api, Resource
from flask import request
from flask_cors import CORS
import pyopentrep, Travel_pb2

def checkCorrectSyntax(query):
    elements = query.split(" ")
    good = all(len(e)==3 for e in elements)
    return good

def checkCities(places):
    codes = []
    for place in places:
        try:
            codes.append({'code':"MAD"})
        except KeyError, e:
            return None
    return codes

app = Flask("Search")
api = Api(app)
cors = CORS(app)

class georequest(Resource):
    def get(self):
        query = request.args.get('query')
        print query
        if checkCorrectSyntax(query):
            places = query.split(" ")
            codes = checkCities(places)
            if codes is not None:
                return {'result': codes}
            else:
                return {'result': [], 'error': "One of the codes was not found"}
        else:
            return {'result': [], 'error': "Incorrect Syntax"}

class randomairportrequest(Resource):
    def get(self):
        return {'result': {'code':"MAD"}} ## It should return info on a random airport

class randomrouterequest(Resource):
    def get(self):
        return {'result': {'code':"MAD"}} ## It should return info on a random route

api.add_resource(georequest, '/v1/geo')
api.add_resource(randomairportrequest, '/v1/randomairport')
api.add_resource(randomrouterequest, '/v1/randomroute')

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=8090)
