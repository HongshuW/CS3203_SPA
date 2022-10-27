import os
from pathlib import Path
import xml.etree.ElementTree as ET

m2path = './milestone-2/milestone-2-out'
m1path = './milestone-1/m1-out'
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
                        m1AllPass = False


    if m2AllPass == False or m1AllPass == False:
        print("fail")
    else:
        print("pass")
else:
    print("directory does not exist")
                
    
