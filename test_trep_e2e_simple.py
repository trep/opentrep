#!/usr/bin/env python

import os, json
import pytest
import pyopentrep


def test_e2e_simple():
    """
    Test initializing and searching with OpenTrepLibrary
    """
    # Initialise the OpenTrep C++ library
    porPath = "/usr/share/opentrep/data/por/test_optd_por_public.csv"
    xapianDBPath = "/tmp/opentrep/xapian_traveldb"
    sqlDBType = "sqlite"
    sqlDBConnStr = "/tmp/opentrep/sqlite_travel.db"
    deploymentNb = 0
    flagDontIndexIATAPOR = False
    flagIndexPORInXapian = True
    flagAddPORInDB = True
    logPath = "/tmp/opentrep/test_trep_e2e_simple.log"
    
    openTrepLibrary = pyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init (porPath, xapianDBPath,
                                   sqlDBType, sqlDBConnStr,
                                   deploymentNb,
                                   flagDontIndexIATAPOR, flagIndexPORInXapian,
                                   flagAddPORInDB,
                                   logPath)
    nce_sfo_query = "nce sfo"
    nce_sfo_result = openTrepLibrary.search("S", nce_sfo_query)
    assert nce_sfo_result, (
        f"The results for the query ({nce_sfo_query}, are not as expected.\n"
        f"Expected: 'NCE/0,SFO/0' expected) - Got: '{nce_sfo_result}'"
    )
