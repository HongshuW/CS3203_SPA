import os
import sys
from pathlib import Path
import xml.etree.ElementTree as ET

path_arg = sys.argv[1]
samplepath =  path_arg + '/Team41/Code41/tests'
m2path = path_arg + '/Team41/Tests41/milestone-2/milestone-2-out'
m1path = path_arg + '/Team41/Tests41/milestone-1/system-test-out'
m3path = path_arg + '/Team41/Tests41/milestone-3/milestone-3-out'

env_file = os.getenv('GITHUB_ENV')

failed_files = []

if (os.path.exists(samplepath) and os.path.exists(m2path) and os.path.exists(m1path) and os.path.exists(m3path)):
    m2AllPass = True
    m1AllPass = True
    m3AllPass = True
    sampleAllPass = True
   
    mydirm2 = Path(m2path)
    for file in mydirm2.glob('*.xml'):
        # parse xml file
        tree = ET.parse(file)
        root = tree.getroot()
        for queries in root.findall("queries"):
            for query in queries:
                for q_tag in query:
                    if q_tag.tag == "failed" or q_tag.tag == "timeout" or q_tag.tag == "exception" or q_tag.tag == "crash":
                        m2 = "milestone-2-out/"
                        failed_files.append(m2 + os.path.basename(file))
                        m2AllPass = False

    mydirm1 = Path(m1path)
    for file in mydirm1.glob('*.xml'):
    # parse xml file
        m1tree = ET.parse(file)
        root = m1tree.getroot()
        for queries in root.findall("queries"):
            for query in queries:
                for q_tag in query:
                    if q_tag.tag == "failed" or q_tag.tag == "timeout" or q_tag.tag == "exception" or q_tag.tag == "crash":
                        m1 = "system-test-out/"
                        failed_files.append(m1 + os.path.basename(file))
                        m1AllPass = False
                        
    mydirm3 = Path(m3path)
    for file in mydirm3.glob('*.xml'):
    # parse xml file
        m3tree = ET.parse(file)
        root = m3tree.getroot()
        for queries in root.findall("queries"):
            for query in queries:
                for q_tag in query:
                    if q_tag.tag == "failed" or q_tag.tag == "timeout" or q_tag.tag == "exception" or q_tag.tag == "crash":
                        m3 = "milestone-3-out/"
                        failed_files.append(m3 + os.path.basename(file))
                        m3AllPass = False
                        
    mydirsample = Path(samplepath)
    for file in mydirsample.glob('*.xml'):
    # parse xml file
        sampletree = ET.parse(file)
        root = sampletree.getroot()
        for queries in root.findall("queries"):
            for query in queries:
                for q_tag in query:
                    if q_tag.tag == "failed" or q_tag.tag == "timeout" or q_tag.tag == "exception" or q_tag.tag == "crash":
                        sample = "sample-out/"
                        failed_files.append(sample + os.path.basename(file))
                        sampleAllPass = False

    if m2AllPass == False or m1AllPass == False or m3AllPass == False or sampleAllPass == False:
        failed_files = set(failed_files)
        fail_msg = "fail_message=The system tests that failed are: "
        for f in failed_files:
            fail_msg+= "--"
            fail_msg += f
            fail_msg += "-- "
        with open(env_file, "a") as myfile:
            myfile.write("check_status=fail\n")
            myfile.write(fail_msg)
    else:
        msg = "pass_message=All system tests passed!"
        with open(env_file, "a") as myfile:
            myfile.write("check_status=pass\n")
            myfile.write(msg)
else:
    print("directory does not exist")
