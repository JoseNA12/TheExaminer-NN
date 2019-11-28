'''
    Todo el código aqui presente fue elaborado e inspirado al elaborado por Moses Olafenwa,
    en su publicación del siguiente enlace:
        https://medium.com/deepquestai/train-object-detection-ai-with-6-lines-of-code-6d087063f6ff
    Autor original: Moses Olafenwa.
    Colaborador oficial de ImageIA.
'''

from imageai.Detection.Custom import DetectionModelTrainer

def entrenarModelo():
    trainer = DetectionModelTrainer()
    trainer.setModelTypeAsYOLOv3()
    trainer.setDataDirectory(data_directory="2_RESPUESTA")
    trainer.setTrainConfig(
        object_names_array=["RESPUESTA"], 
                        batch_size=2, 
                        num_experiments=50, 
                        train_from_pretrained_model="pretrained-yolov3.h5")
    trainer.trainModel()

def evaluarModelo():
    trainer = DetectionModelTrainer()
    trainer.setModelTypeAsYOLOv3()
    trainer.setDataDirectory(data_directory="2_RESPUESTA")
    metrics = trainer.evaluateModel(model_path="models", json_path="detection_config.json", iou_threshold=0.5, object_threshold=0.3, nms_threshold=0.5)
    print(metrics)

def evaluarImagen():
    detector = CustomObjectDetection()
    detector.setModelTypeAsYOLOv3()
    detector.setModelPath("detection_model-ex-002--loss-0150.270.h5")
    detector.setJsonPath("detection_config.json")
    detector.loadModel()
    detections = detector.detectObjectsFromImage(input_image="img (11).jpg", output_image_path="img-detected.jpg")
    for detection in detections:
        print(detection["name"], " : ", detection["percentage_probability"], " : ", detection["box_points"])