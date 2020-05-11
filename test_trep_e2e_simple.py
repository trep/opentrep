#!/usr/bin/env python

import os, json
import pytest
import pyopentrep


def test_e2e_simple():
    """
    Test initializing and searching with OpenTrepLibrary
    """
    openTrepLibrary = pyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init(
        "/tmp/opentrep/xapian_traveldb",
        "sqlite",
        "/tmp/opentrep/sqlite_travel.db",
        0,
        "pyopentrep.log",
    )
    nce_sfo_query = "nce sfo"
    nce_sfo_result = penTrepLibrary.search("S", nce_sfo_query)
    assert nce_sfo_result, (
        f"The results for the query ({nce_sfo_query}, are not as expected.\n"
        f"Expected: 'NCE/0,SFO/0' expected) - Got: '{nce_sfo_result}'"
    )
