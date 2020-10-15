va_list list;
std::string out;
va_start(list, str);

for (uint32_t i = 0; i < str.size(); i++)
{
	if(str[i] == '%')
	{
		i++;
		switch (str[i])
		{
			// char
			case 'c':
				out += (char)va_arg(list, int);
				break;

			// string
			case 's':
				out += std::string(va_arg(list, char*));
				break;
	
			// float/double
			case 'f':
				out += std::to_string(va_arg(list, double));
				break;
	
			// decimal
			case 'd':
				out += std::to_string(va_arg(list, int));
				break;
	
			// percent sign
			case '%':
				out += '%';
				break;
	
			default :
				out += ' ';
				break;
		}
	}
	else
	{
		out += str[i];
	}
}

va_end(list);
