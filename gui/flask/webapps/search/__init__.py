from flask import Flask
from flask_restful import Api, Resource
from flask import request
from flask_cors import CORS
import json
import pyopentrep, Travel_pb2

# Initialise the OpenTrep C++ library
trep_dir = "/var/www/webapps/opentrep/trep"
porPath = f"{trep_dir}/share/opentrep/data/por/test_optd_por_public.csv"
# xapianDBPath = "/tmp/opentrep/xapian_traveldb"
# sqlDBConnStr = "/tmp/opentrep/sqlite_travel.db"
xapianDBPath = f"{trep_dir}/traveldb"
sqlDBType = "sqlite"
sqlDBConnStr = f"{trep_dir}/sqlite_travel.db"
#sqlDBType = "mysql"
#sqlDBConnStr = "db=trep_trep user=trep password=trep"
deploymentNb = 0
flagDontIndexIATAPOR = False
flagIndexPORInXapian = True
flagAddPORInDB = True
logPath = "/var/log/webapps/search/pyopentrep.log"
openTrepLibrary = pyopentrep.OpenTrepSearcher()
initOK = openTrepLibrary.init (porPath, xapianDBPath, sqlDBType, sqlDBConnStr,
                               deploymentNb,
                               flagDontIndexIATAPOR, flagIndexPORInXapian,
                               flagAddPORInDB,
                               logPath)

# Generate a query string with random place codes
def generateRandomString (nbOfDraws):
  # Randomly generate one place
  result = json.loads(openTrepLibrary.generate('J', nbOfDraws))
  return result


app = Flask("Search")
api = Api(app)
cors = CORS(app)

class georequest(Resource):
    def get(self):
        query_str = request.args.get('query')
        query_string_str = query_str.encode("utf8")
        result = json.loads(openTrepLibrary.search('J', query_string_str))
        return {'result': result}

class randomairportrequest(Resource):
    def get(self):
        return {'result':  generateRandomString(1) } ## It should return info on a random airport

class randomrouterequest(Resource):
    def get(self):
        return {'result': generateRandomString(3) } ## It should return info on a random route

api.add_resource(georequest, '/v1/geo')
api.add_resource(randomairportrequest, '/v1/randomairport')
api.add_resource(randomrouterequest, '/v1/randomroute')

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=8090)
