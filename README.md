# 3D Mandible Segmentation 
An Optimized Technique of 3D Segmentation for Orthodontic Treatment Plan. 
# Build 

Dependencies:

* VTK 7.1
* Qt5
* OpenMP

```bash
$ cd mandible-segmentation
$ mkdir build 
$ cd build
$ cmake .. 
$ make -j 4
```

The executable files will be under the following directory.

```bash
mandible-segmentation/bin
```

## Loading DICOM Series

![](docs/images/report2/axial.png)
![](docs/images/report2/cronal.png)
![](docs/images/report2/sagittal.png)

## Extracting ROI and Volume Thresholding

![](docs/images/report3/T1100.png)
![](docs/images/report4/BSS.png)


## 3D Breadth First Search (BFS) for mandible segmentation


![](docs/images/report4/RCS.png)
![](docs/images/report4/MCS.png)

## Teeth Segmentation

![](docs/images/report5/MCF1.png)

## UI Tool 

![](docs/images/report7/C15L.png)
![](docs/images/report7/C15U.png)
