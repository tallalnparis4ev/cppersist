const NewManager = require('./dbmanagers/newManager.js')
const ConsistentManager = require('./dbmanagers/consistentManager.js')
const Express = require("express");
const BodyParser = require("body-parser");
const MongoClient = require("mongodb").MongoClient;
const CONNECTION_URL = "mongodb://localhost:27017";
const DATABASE_NAME = "memoisation";
const PORT = 5001; //Different to EVE

//Maintains consistency with pypersist and GAP memoization
const consistent = process.argv.includes("--consistent")

var app = Express();
app.use(BodyParser.json());
app.use(BodyParser.urlencoded({ extended: true }));
var database, dbManager;

const log = (str) => {
  console.log(str);
}

app.listen(PORT, () => {
    log(`Listening on ${PORT}`)
    MongoClient.connect(CONNECTION_URL, { useNewUrlParser: true, useUnifiedTopology: true}, (error, client) => {
        if(error) {
            throw error;
        }
        database = client.db(DATABASE_NAME);
        console.log("Connected to `" + DATABASE_NAME + "`!");
        if(consistent) dbManager = new ConsistentManager(database);
        else dbManager = new NewManager(database);
        log(consistent);
    });
});

app.get("/:function/:id", (request, response) => {
  log("GET REQUEST:")
  log(request.params.function + "," + request.params.id);
  dbManager.handleGet(request,response);
});

app.post("/:function/:id", (request, response) => {
  log("POST REQUEST")
  log(request.params.function + "," + request.params.id)  
  log(request.body)
  dbManager.handlePost(request,response);
});
