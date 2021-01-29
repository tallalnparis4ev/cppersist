namespace_t = ["gapmemo", "pypersist", "cppersist"]

memo_schema = {
    # Required fields
    "funcname": {"type": "string", "minlength": 1, "required": True},
    "hash": {"type": "string", "required": True},
    "namespace": {"type": "string", "required": True},
    "result": {"type": "string", "required": True},
    # Optional fields
    "key": {"type": "string", "required": False},
    "metadata": {"type": "string", "required": False},
    "comments": {"type": "string", "required": False},
}

persist = {
    "url": "persist/<regex('[\\w]+'):funcname>",
    # by default the standard item entry point is defined as
    # "/people/<ObjectId>". We leave it untouched, and we also enable an
    # additional read-only entry point. This way consumers can also perform
    # GET requests at "/people/<lastname>".
    "additional_lookup": {"url": "regex('.+')", "field": "hash"},
    # We choose to override global cache-control directives for this resource.
    # "cache_control": "max-age=10,must-revalidate",
    # "cache_expires": 10,
    # most global settings can be overridden at resource level
    "resource_methods": ["GET", "POST", "DELETE"],
    "schema": memo_schema,
}

MONGO_HOST = "localhost"
MONGO_PORT = 27017

MONGO_DBNAME = "memoisation"  # Database name
DOMAIN = {"persist": persist}  # Collection name

RESOURCE_METHODS = ["GET", "POST", "DELETE"]
ITEM_METHODS = ["GET", "PATCH", "PUT", "DELETE"]
