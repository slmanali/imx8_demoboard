# Code Documentation for `Model_ManDown.py`

## Overview

`Model_ManDown.py` is a Python script that implements a machine learning model using a Random Forest classifier from the scikit-learn library to classify movements from sensor data. The model is trained on features and labels read from external text files, evaluated for performance, and exported to the ONNX format for easier deployment. The script uses various libraries for model building, evaluation, and conversion.

## Libraries and Dependencies

The script relies on the following Python libraries:

- `sklearn`: For machine learning functionalities, particularly the Random Forest classifier and tools for model evaluation.
- `skl2onnx`: To convert the trained scikit-learn model into the ONNX (Open Neural Network Exchange) format.
- `joblib`: For saving and loading Python objects, specifically for saving the trained machine learning model.
- `numpy`: For numerical operations and array handling.
- `matplotlib`: Although imported, it is not used in this script but might be intended for plotting results.

## Code Breakdown

### Imports

```python
import skl2onnx
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType
import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import RandomForestClassifier
import joblib
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, confusion_matrix
```

### Constants Definition

```python
print('start')

RANDOM_SEED = 0
NUMTREES = 200
```

- `RANDOM_SEED`: A constant for reproducibility of results by controlling randomness.
- `NUMTREES`: The number of trees to be used in the Random Forest classifier.

### Data Loading

```python
x_train_file = open('Class_features.txt', 'r')
y_train_file = open('Class_labels.txt', 'r')
```

- Opens two text files: one containing the feature data (`Class_features.txt`) and the other containing the corresponding labels (`Class_labels.txt`).

### Data Processing

```python
# Create empty lists
x_train = []
y_train = []

labels = {'Work': 1, 'Relax': 2, 'Fall': 3}
```

- Initializes empty lists to store feature data (`x_train`) and label data (`y_train`).
- A mapping dictionary (`labels`) is defined to associate class names with integer labels.

#### Reading Features

```python
for x in x_train_file:
    x_train.append([float(ts) for ts in x.split()])
```

- Reads each line of `Class_features.txt`, splits it into individual tokens, converts them to floats, and appends them to the `x_train` list.

#### Reading Labels

```python
for y in y_train_file:
    y_train.append(int(y.rstrip('\n')))
```

- Reads each line of `Class_labels.txt` and converts it to an integer after stripping the newline character, appending it to `y_train`.

### Data Conversion and Splitting

```python
x_train = np.array(x_train)
y_train = np.array(y_train)
print(x_train.shape)  # should print (n_samples, 17)
X_train, X_test, y_train, y_test = train_test_split(
    x_train, y_train, test_size=0.3)
```

- Converts `x_train` and `y_train` lists into NumPy arrays for easier manipulation.
- The shape of the feature array is printed, which should indicate the number of samples and the number of features (17).
- Uses `train_test_split` to split the dataset into training (70%) and testing (30%) sets.

### Model Training

```python
rf = RandomForestClassifier(criterion='gini', n_estimators=NUMTREES)
rf.fit(X_train, y_train)
y_pred = rf.predict(X_test)
```

- Initializes a Random Forest classifier with 'gini' as the criterion for splitting and specifies the number of estimators (trees).
- Fits the model on the training data (`X_train`, `y_train`).
- Makes predictions on the test set (`X_test`) and stores them in `y_pred`.

### Model Evaluation

```python
conf_mat = confusion_matrix(y_test, y_pred)
print(classification_report(y_test, y_pred))
```

- Calculates the confusion matrix to evaluate the performance of the classifier.
- Prints a classification report that includes precision, recall, F1-score, and support for each class.

### Model Saving and Conversion to ONNX

```python
joblib.dump(rf, "Class_Freq_R.pkl")
initial_type = [('float_input', FloatTensorType([None, 18]))]  # 17 if that's your feature size
onnx_model = convert_sklearn(rf, initial_types=initial_type)
with open("Class_Freq_R.onnx", "wb") as f:
    f.write(onnx_model.SerializeToString())
```

- Saves the trained Random Forest model as a serialized `.pkl` file using `joblib`.
- Defines the initial input type for the ONNX model conversion based on the expected feature size (18 in this case, which appears to be a potential oversight since the shape indicates 17).
- Converts the trained model to ONNX format and writes it to a file named `Class_Freq_R.onnx`.

### Completion Message

```python
print('Finish')
```

- Prints a message indicating that the script has finished execution.

### Optional ONNX Model Inspection (Commented Out)

```python
# import onnx
# model = onnx.load("Class_Freq_R.onnx")
# print("Inputs:")
# for i in model.graph.input:
#     print(i.name, i.type.tensor_type.shape)
# print("Outputs:")
# for o in model.graph.output:
#     print(o.name, o.type.tensor_type.shape)
```

- The commented-out section can be used to load and inspect the structure of the ONNX model, printing out the input and output tensor shapes, but is not executed in the current script.

## Conclusion

`Model_ManDown.py` efficiently handles the process of training a Random Forest classifier, evaluating its performance, and exporting the trained model to the ONNX format for potential deployment in a wider range of applications. The script serves as a foundational template for future machine learning projects, ensuring clarity and structure in data processing and model management.

---
