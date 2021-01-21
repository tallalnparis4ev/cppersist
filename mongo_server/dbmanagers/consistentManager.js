class ConsistentManager {
  constructor(database) {
    this.collection = database.collection("persist");
  }
  handlePost(request, response) {
    const insert = {
      funcname:request.params.function,
      hash:request.params.id,
      namespace:"cppersist",
      result:request.body.return
    }
    this.collection.insertOne(insert, function(err, res) {
      if (err) console.log("error putting");
      response.sendStatus(200)
    });
  }
  handleGet(request, response){
    this.collection.findOne({"hash": request.params.id }, (error, result) => {
      if(error || (result == null)) 
        response.sendStatus(500);
      else
        response.send(result.result);
    });
  }
}
module.exports = ConsistentManager
