#pragma once





/*
static enum DATA_DEFINE_ID {
	DEFINITION_PDR,
};

static enum DATA_REQUEST_ID {
	REQUEST_PDR,
};

static enum DATA_NAMES {
	DATA_VERTICAL_SPEED,
	DATA_PITOT_HEAT
};
*/

/* varName and unitName can be found here : http://msdn.microsoft.com/en-us/library/cc526981.aspx
*/
struct structVarUnit{
	char* varName;
	char* unitName;
};
