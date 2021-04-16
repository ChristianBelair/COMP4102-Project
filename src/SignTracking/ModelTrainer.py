import csv
import cv2 as cv
import numpy as np
import os
import random

bin_n = 16 # Number of bins
crop_size = 64


def hog(img):
    gx = cv.Sobel(img, cv.CV_32F, 1, 0)
    gy = cv.Sobel(img, cv.CV_32F, 0, 1)
    mag, ang = cv.cartToPolar(gx, gy)
    bins = np.int32(bin_n*ang/(2*np.pi))    # quantizing binvalues in (0...16)
    bin_cells = bins[:10,:10], bins[10:,:10], bins[:10,10:], bins[10:,10:]
    mag_cells = mag[:10,:10], mag[10:,:10], mag[:10,10:], mag[10:,10:]
    hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
    hist = np.hstack(hists)     # hist is a 64 bit vector
    return hist

def getTrainingData():
    data = []
    labels = []
    for i in range(0, 1):
        print("\tloading set " + str(i))
        with open("trainingData/signData/vid" + str(i) + "/frameAnnotations.csv") as f: 
            reader = csv.reader(f, delimiter=';')
            next(reader, None)
            for row in reader:
                img = cv.imread("trainingData/signData/vid" + str(i) + "/" + row[0], cv.IMREAD_GRAYSCALE)
                cropped = img[int(row[3]):int(row[5]), int(row[2]):int(row[4])]
                ho = hog(cropped)
                data.append(ho)
                labels.append(1)
    """
    print("\tloading negative set")
    for i in range(0, 2000):
        img = cv.imread("trainingData/signData/negatives/negativePics/nosign" + str(i).zfill(5) + ".png", cv.IMREAD_GRAYSCALE)
        x = random.randint(20, len(img) - 70)
        y = random.randint(20, len(img[0]) - 70)
        cropped = img[x:x+crop_size, y:y+crop_size]
        ho = hog(cropped)
        data.append(ho)
        labels.append(0)
    """
    
    return data, labels


print("Loading data")
data, labels = getTrainingData()

print(len(data[0]))

data = np.float32(data)
labels = np.array(labels)

print("Done")

svm = cv.ml.SVM_create()
svm.setKernel(cv.ml.SVM_LINEAR)
svm.setType(cv.ml.SVM_C_SVC)
print("Beginning training")
svm.trainAuto(data, cv.ml.ROW_SAMPLE, labels)

svm.save("svm_data.xml")
print("Done")

"""
svm = cv.ml_SVM()
svm.load("svm_data.xml")
img = cv.imread("stop.png")
hog = cv.HOGDescriptor()
hog.setSVMDetector()
found, weights = hog.detectMultiScale(img)
print(found)
"""