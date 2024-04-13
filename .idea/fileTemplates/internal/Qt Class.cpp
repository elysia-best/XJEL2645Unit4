/*
 * (C) Copyright ${YEAR} Yuanzhen Gan. All Rights Reserved.
 * Description: ${FILE_NAME}
 * Author: Yuanzhen Gan
 * Date: ${DATE}
 * Modify Record:
 */
#parse("C File Header.h")
// You may need to build the project (run Qt uic code generator) to get "${UI_HEADER_FILENAME}" resolved

#[[#include]]# "${HEADER_FILENAME}"
#[[#include]]# "${UI_HEADER_FILENAME}"

${USER_BEGIN_NAMESPACE}
${NAME}::${NAME}(QWidget *parent) :
    ${PARENT_CLASS}(parent), ui(new Ui::${NAME}) {
    ui->setupUi(this);
}

${NAME}::~${NAME}() {
    delete ui;
}
${USER_END_NAMESPACE}
