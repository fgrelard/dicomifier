#########################################################################
# Dicomifier - Copyright (C) Universite de Strasbourg
# Distributed under the terms of the CeCILL-B license, as published by
# the CEA-CNRS-INRIA. Refer to the LICENSE file or to
# http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
# for details.
#########################################################################

GeneralEquipment = [ # http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.7.5.html#sect_C.7.5.1
    ("ORIGIN", "Manufacturer", 2, None, None),
    ("VisuInstitution", "InstitutionName", 3, None, None),
    ("VisuStation", "StationName", 3, None, None),
    (
        None, "SoftwareVersions", 3, 
        lambda d,g,i: reduce(
            lambda e1, e2: e1+e2,
            [d.get(name, []) for name in ["VisuCreator", "VisuCreatorVersion"]], 
            []
        ),
        None
    ),
]