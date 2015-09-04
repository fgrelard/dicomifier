require('bruker2dicom/common.js');

var _module = namespace('dicomifier.bruker2dicom.functionalGroups');

_module.PlanePositionPatient = function(indexGenerator, dicomSequenceItem, 
                                        brukerDataset) {
    var toDicom = dicomifier.bruker2dicom.toDicom;
    
    var item = {};
    
    var indexOrentation = dicomifier.bruker2dicom.getFrameGroupIndex(brukerDataset, 'VisuCorePosition');
    var beginIndex = indexOrentation[1] + indexGenerator.currentIndex[indexOrentation[0]] * 3;
    var value = brukerDataset['VisuCorePosition'].slice(beginIndex, beginIndex + 3);
    item[dicomifier.dictionary['ImagePositionPatient'][1]] = {
        'vr': dicomifier.dictionary['ImagePositionPatient'][0], 
        'Value' : value };
    
    dicomSequenceItem[dicomifier.dictionary['PlanePositionSequence'][1]] = {
        'vr': dicomifier.dictionary['PlanePositionSequence'][0], 
        'Value' : [ item ] };
};
