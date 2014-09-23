"""
"   Author: Ying Xiong.
"   Created: Jun 18, 2013.
"""

import os

if __name__ == "__main__":
    command = "./visualizePointsAndCameras \\\n\t" + \
              "--pointFileType=ply " + \
              "--pointFile=TestData/Models/dinoSparseRing-pmvs.ply \\\n\t" + \
              "--cameraFileType=NumNameKRt " + \
              "--cameraFile=TestData/Models/dinoSparseRing-cams.txt"
    print(command)
    os.system(command)
