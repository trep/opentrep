#!/usr/bin/env python

import os, json, urllib.request, shutil, pathlib
import pytest
import pyopentrep


def test_e2e_simple():
    """
    Test initializing and searching with OpenTrepLibrary
    """
    # Initialise the OpenTrep C++ library
    tmp_dir = "/tmp/opentrep"
    optd_por_test_url = \
        'https://github.com/trep/opentrep/blob/master/data/por/test_optd_por_public.csv?raw=true'
    porPath1 = "/usr/share/opentrep/data/por/test_optd_por_public.csv"
    porPath2 = "/usr/local/share/opentrep/data/por/test_optd_por_public.csv"
    porPath3 = f"{tmp_dir}/test_optd_por_public.csv"
    porPath = None
    xapianDBPath = f"{tmp_dir}/xapian_traveldb"
    sqlDBType = "sqlite"
    sqlDBConnStr = f"{tmp_dir}/sqlite_travel.db"
    deploymentNb = 0
    flagDontIndexIATAPOR = False
    flagIndexPORInXapian = True
    flagAddPORInDB = True
    logPath = f"{tmp_dir}/test_trep_e2e_simple.log"

    # Create the OpenTREP temporary directory if not already existing
    pathlib.Path(tmp_dir).mkdir(parents=True, exist_ok=True)
    
    # If the POR sample data cannot be found, download it
    porFileExists = os.path.exists (porPath1)
    if porFileExists:
        # The POR sample data has been found
        porPath = porPath1
    else:
        # Try the other location for the POR sample data
        porFileExists = os.path.exists (porPath2)
    if porFileExists:
        # The POR sample data has been found
        porPath = porPath2
    else:        
        # No POR sample data has been found. Download it
        with urllib.request.urlopen (optd_por_test_url) as response, \
             open (porPath3, 'wb') as out_file:
            shutil.copyfileobj (response, out_file)
        porPath = porPath3

    #
    porFileExists = os.path.exists (porPath)
    assert porFileExists, (
        f"The POR sample data file does not seem to exist"
    )
    
    #
    openTrepLibrary = pyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init (porPath, xapianDBPath,
                                   sqlDBType, sqlDBConnStr,
                                   deploymentNb,
                                   flagDontIndexIATAPOR, flagIndexPORInXapian,
                                   flagAddPORInDB,
                                   logPath)
    
    # Retrieve the file-paths of all the OpenTREP resources
    expectedFPList = f"{porPath};{xapianDBPath}{deploymentNb};{sqlDBConnStr}{deploymentNb}"
    filePathListString = openTrepLibrary.getPaths()
    assert filePathListString == expectedFPList, (
        f"File-paths: {filePathListString}"
    )
    
    # First, create the Xapian index
    expectedNbOfPOR = "9"
    nb_of_por = openTrepLibrary.index()
    assert nb_of_por == expectedNbOfPOR, (
        f"Number of index POR: {nb_of_por}"
    )

    # Search a simple query on the just created Xapian index
    expectedResult = "NCE/0,SFO/0"
    nce_sfo_query = "nce sfo"
    nce_sfo_result = openTrepLibrary.search("S", nce_sfo_query)
    assert nce_sfo_result == expectedResult, (
        f"The results for the query ({nce_sfo_query}, are not as expected.\n"
        f"Expected: 'NCE/0,SFO/0' expected) - Got: '{nce_sfo_result}'"
    )
