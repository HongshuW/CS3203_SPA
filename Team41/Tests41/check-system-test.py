import os
import sys
from pathlib import Path
import xml.etree.ElementTree as ET

path_arg = sys.argv[1]
m2path = path_arg + '/Team41/Tests41/milestone-2/milestone-2-out'
m1path = path_arg + '/Team41/Tests41/milestone-1/system-test-out'

env_file = os.getenv('GITHUB_ENV')

failed_files = []

if (os.path.exists(m2path) and os.path.exists(m1path)):
    m2AllPass = True
    m1AllPass = True
   
    mydirm2 = Path(m2path)
    for file in mydirm2.glob('*.xml'):
        # parse xml file
        tree = ET.parse(file)
        root = tree.getroot()
        for queries in root.findall("queries"):
            for query in queries:
                for q_tag in query:
                    if q_tag.tag == "failed":
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
                    if q_tag.tag == "failed":
                        m1 = "system-test-out/"
                        failed_files.append(m1 + os.path.basename(file))
                        m1AllPass = False

    if m2AllPass == False or m1AllPass == False:
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
