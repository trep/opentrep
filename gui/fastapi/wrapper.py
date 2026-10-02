import json
import os
import sys

def search_subprocess(q, p, x, d):
    from pyopentrep.pyopentrep import OpenTrepSearcher
    _trep = OpenTrepSearcher()
    LOG_PATH = f"/var/log/webapps/search/pyopentrep_{os.getpid()}.log"
    _trep.init(p, x, "nodb", "", d, False, True, False, LOG_PATH)
    res = _trep.search("J", q)
    return res

def generate_subprocess(t, n, p, x, d):
    from pyopentrep.pyopentrep import OpenTrepSearcher
    _trep = OpenTrepSearcher()
    LOG_PATH = f"/var/log/webapps/search/pyopentrep_{os.getpid()}.log"
    _trep.init(p, x, "nodb", "", d, False, True, False, LOG_PATH)
    res = _trep.generate(t, n)
    return res

if __name__ == "__main__":
    op = sys.argv[1]

    if op == "search":
        q = sys.argv[2]
        p = sys.argv[3]
        x = sys.argv[4]
        d = int(sys.argv[5])
        print(search_subprocess(q, p, x, d))
    elif op == "generate":
        t = sys.argv[2]
        n = int(sys.argv[3])
        p = sys.argv[4]
        x = sys.argv[5]
        d = int(sys.argv[6])
        print(generate_subprocess(t, n, p, x, d))
