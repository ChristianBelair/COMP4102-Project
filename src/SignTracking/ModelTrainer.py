import csv
import cv2 as cv
import numpy as np
import os
import random

bin_n = 16 # Number of bins
crop_size = 64


# This one's fucked
def hog(img):
    print(img.size())
    gx = cv.Sobel(img, cv.CV_32F, 1, 0)
    gy = cv.Sobel(img, cv.CV_32F, 0, 1)
    mag, ang = cv.cartToPolar(gx, gy)
    bins = np.int32(bin_n*ang/(2*np.pi))    # quantizing binvalues in (0...16)
    bin_cells = bins[:10,:10], bins[10:,:10], bins[:10,10:], bins[10:,10:]
    mag_cells = mag[:10,:10], mag[10:,:10], mag[:10,10:], mag[10:,10:]
    hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
    hist = np.hstack(hists)     # hist is a 64 bit vector
    return hist

# This one just finds zeros
def hog2(img):
    winSize = (64,64)
    blockSize = (16,16)
    blockStride = (8,8)
    cellSize = (8,8)
    nbins = 9
    derivAperture = 1
    winSigma = 4.
    histogramNormType = 0
    L2HysThreshold = 2.0000000000000001e-01
    gammaCorrection = 0
    nlevels = 64
    hog = cv.HOGDescriptor(winSize,blockSize,blockStride,cellSize,nbins,derivAperture,winSigma,
                            histogramNormType,L2HysThreshold,gammaCorrection,nlevels)
    #compute(img[, winStride[, padding[, locations]]]) -> descriptors
    winStride = (8,8)
    padding = (8,8)
    locations = ((10,20),)
    hist = hog.compute(img,winStride,padding,locations)
    return hist

# I think I can get this one working
def hog3(img):
    winSize = (16,16)
    blockSize = (4,4)
    blockStride = (2,2)
    cellSize = (2,2)
    nbins = 9
    derivAperture = 1
    winSigma = -1.
    histogramNormType = 0
    L2HysThreshold = 0.2
    gammaCorrection = 1
    nlevels = 64
    signedGradients = True
    hog = cv.HOGDescriptor(winSize,blockSize,blockStride,
        cellSize,nbins,derivAperture,
        winSigma,histogramNormType,L2HysThreshold,
        gammaCorrection,nlevels, signedGradients)
    #compute(img[, winStride[, padding[, locations]]]) -> descriptors
    winStride = (4,4)
    padding = (0,0)
    locations = ((10,20),)
    hist = hog.compute(img,winStride,padding,locations)
    return hist

def getTrainingData():
    data = []
    labels = []
    for i in range(0, 12):
        print("\tloading set " + str(i))
        with open("trainingData/signData/vid" + str(i) + "/frameAnnotations.csv") as f: 
            reader = csv.reader(f, delimiter=';')
            next(reader, None)
            for row in reader:
                img = cv.imread("trainingData/signData/vid" + str(i) + "/" + row[0], cv.IMREAD_GRAYSCALE)
                if int(row[5]) - int(row[3]) < 64 and int(row[4]) - int(row[2]) < 64:
                    offx = 64 - (int(row[5]) - int(row[3]))
                    offy = 64 - (int(row[4]) - int(row[2]))
                    if int(row[3]) - int(offx/2) > 0 and int(row[5]) + int(offx/2) < len(img) and int(row[2]) - int(offy/2) > 0 and int(row[4]) - int(offy/2) < len(img[0]):
                        cropped = img[int(row[3]) - int(offx/2):int(row[5]) + int(offx/2), int(row[2]) - int(offy/2):int(row[4]) + int(offy/2)]
                        ho = hog2(cropped)
                        data.append(ho)
                        labels.append(1)

    print("\tloading negative set")
    for i in range(0, 2000):
        img = cv.imread("trainingData/signData/negatives/negativePics/nosign" + str(i).zfill(5) + ".png", cv.IMREAD_GRAYSCALE)
        x = random.randint(20, len(img) - 70)
        y = random.randint(20, len(img[0]) - 70)
        cropped = img[x:x+crop_size, y:y+crop_size]
        ho = hog2(cropped)
        data.append(ho)
        labels.append(-1)
    
    return data, labels

def getTrainingData3():
    data = []
    labels = []
    for i in range(0, 12):
        print("\tloading set " + str(i))
        with open("trainingData/signData/vid" + str(i) + "/frameAnnotations.csv") as f: 
            reader = csv.reader(f, delimiter=';')
            next(reader, None)
            for row in reader:
                img = cv.imread("trainingData/signData/vid" + str(i) + "/" + row[0], cv.IMREAD_COLOR)
                cropped = img[int(row[3]):int(row[5]), int(row[2]):int(row[4])]
                ho = hog3(cropped)
                data.append(ho)
                labels.append(1)

    print("\tloading negative set")
    for i in range(0, 4000):
        img = cv.imread("trainingData/signData/negatives/negativePics/nosign" + str(i).zfill(5) + ".png", cv.IMREAD_COLOR)
        ho = hog3(img)
        data.append(ho)
        labels.append(0)
    
    return data, labels


print("Loading data")
#data, labels = getTrainingData()
data, labels = getTrainingData3()

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