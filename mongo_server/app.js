const Express = require("express");
const BodyParser = require("body-parser");
const MongoClient = require("mongodb").MongoClient;
const ObjectId = require("mongodb").ObjectID;
const CONNECTION_URL = "mongodb://localhost:27017";
const DATABASE_NAME = "cppersist";
let curCollection = ""
const sameCollection = process.argv[2];
const useSameCollection = !!sameCollection
var app = Express();
app.use(BodyParser.json());
app.use(BodyParser.urlencoded({ extended: true }));
var database, collection;

const setCollection = (newCollection) => {
  if (newCollection != curCollection){
    collection = database.collection(newCollection);
    curCollection = newCollection;
  }
}

const log = (str) => {
  // console.log(str);
}

app.listen(5000, () => {
    MongoClient.connect(CONNECTION_URL, { useNewUrlParser: true, useUnifiedTopology: true}, (error, client) => {
        if(error) {
            throw error;
        }
        database = client.db(DATABASE_NAME);
        if(useSameCollection) setCollection(sameCollection);
        console.log("Connected to `" + DATABASE_NAME + "`!");
    });
});

app.get("/:function/:id", (request, response) => {
  if(!useSameCollection) setCollection(request.params.function);
  log("GET REQUEST:")
  log(request.params.function + "," + request.params.id);
  collection.findOne({ "_id": request.params.id }, (error, result) => {
    if(error || (result == null)) 
      response.sendStatus(500);
    else
      response.send(result.return);
  });
});

app.post("/:function/:id", (request, response) => {
  if(!useSameCollection) setCollection(request.params.function);
  log("POST REQUEST")
  log(request.params.function + "," + request.params.id)  
  log(request.body)
  const insert = {_id:request.params.id, return:request.body.return}
  collection.insertOne(insert, function(err, res) {
    if (err) console.log("error putting");
    response.sendStatus(200)
  });
});

