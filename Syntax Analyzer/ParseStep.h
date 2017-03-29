enum ParseMethod{
	PARSE_SHIFT,   //shift
	PARSE_REDUCE,  //reduce
	PARSE_GOTO,    //goto
	PARSE_ACC      //accept   
};

struct ParseStep
{
public:
	ParseMethod method;
	int parseToWhat;
	ParseStep(ParseMethod method,int parseToWhat)
	{
		this->method = method;
		this->parseToWhat = parseToWhat;
	}
};
