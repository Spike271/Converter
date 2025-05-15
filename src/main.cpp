#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include <cctype>
#include <cstring>
#include <format>
#include <stack>
#include <string>
#include "my_application_ui.h"

class ExpressionEvaluate
{
public:
    static std::pair<std::string, std::string> Evaluate(std::string &str, const int value)
    {
        std::pair<std::string, std::string> list;

        if (value == 1)
        {
            list.first = Prefix_to_infix(str);
            list.second = Prefix_to_postfix(str);
        }
        else if (value == 2)
        {
            list.first = Infix_to_prefix(str);
            list.second = Infix_to_postfix(str);
        }
        else if (value == 3)
        {
            list.first = Postfix_to_prefix(str);
            list.second = Postfix_to_infix(str);
        }

        return list;
    }

    static bool isOperator(const char c) { return !std::isalpha(c) && !std::isdigit(c); }

private:
    static int precedence(const char ch)
    {
        if (ch == '^')
            return 3;
        else if (ch == '*' || ch == '/')
            return 2;
        else if (ch == '+' || ch == '-')
            return 1;
        else
            return -1;
    }

    static char associativity(const char ch)
    {
        if (ch == '^')
            return 'R';
        return 'L';
    }

    static std::string Prefix_to_postfix(std::string& str)
    {
        std::stack<std::string> Stack1;
        const uint32_t len = str.length();
        std::string temp, var1, var2;

        for (uint32_t i = 0; i < len; i++)
        {
            temp = "";
            try
            {
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
                {
                    var1 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    var2 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    temp = var1 + var2 + str[i];
                    Stack1.push(temp);
                }
                else
                {
                    temp += str[i];
                    Stack1.push(temp);
                }
            }
            catch (...)
            {
                std::cerr << "Wrong Expression\n";
                return "Wrong Expression";
            }
        }
        return Stack1.top();
    }

    static std::string Prefix_to_infix(std::string& str)
    {
        std::stack<std::string> Stack1;
        std::ranges::reverse(str);
        std::string temp, var1, var2;

        const uint32_t len = str.length();

        for (uint32_t i = 0; i < len; i++)
        {
            temp = "";
            try
            {
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == '%')
                {
                    var1 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    var2 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    temp = std::format("({}{}{})", var1, str[i], var2);
                    Stack1.push(temp);
                }
                else
                {
                    temp += str[i];
                    Stack1.push(temp);
                }
            }
            catch (...)
            {
                std::cerr << "Wrong Expression\n";
                return "Wrong Expression";
            }
        }
        return Stack1.top();
    }

    static std::string Postfix_to_prefix(const std::string& str)
    {
        std::stack<std::string> Stack1;
        std::string temp, var1, var2;
        uint32_t len = str.length();

        try
        {
            for (uint32_t i = 0; i < len; i++)
            {
                temp = "";
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
                {
                    var1 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    var2 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    temp = std::format("{}{}{}", str[i],var2 ,var1);
                    Stack1.push(temp);
                }
                else
                {
                    temp += str[i];
                    Stack1.push(temp);
                }
            }
            temp = "";

            while (!Stack1.empty())
            {
                temp = Stack1.top();
                Stack1.pop();
            }
        }
        catch (...)
        {
            std::cerr << "Wrong Expression\n";
            temp = "Wrong Expression";
        }
        return temp;
    }

    static std::string Postfix_to_infix(const std::string& str)
    {
        std::stack<std::string> Stack1;
        std::string temp, var1, var2;
        const uint32_t len = str.length();

        for (uint32_t i = 0; i < len; i++)
        {
            temp = "";
            try
            {
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == '%')
                {
                    var2 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    var1 = !Stack1.empty() ? Stack1.top() : throw std::out_of_range("Stack is empty");
                    Stack1.pop();

                    temp = std::format("({}{}{})", var1, str[i], var2);
                    Stack1.push(temp);
                }
                else
                {
                    temp += str[i];
                    Stack1.push(temp);
                }
            }
            catch (...)
            {
                std::cerr << "Wrong Expression\n";
                return "Wrong Expression";
            }
        }
        return Stack1.top();
    }

    static std::string Infix_to_postfix(std::string& str)
    {
        std::stack<char> Stack1;
        std::string result;
        const uint32_t len = str.length();

        try
        {
            for (uint32_t i = 0; i < len; i++)
            {
                char temp = str[i];

                if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
                {
                    result += temp;
                }

                else if (temp == '(')
                {
                    Stack1.push('(');
                }

                else if (temp == ')')
                {
                    while (Stack1.top() != '(')
                    {
                        result += Stack1.top();
                        Stack1.pop();
                    }
                    Stack1.pop();
                }

                else
                {
                    while (!Stack1.empty() && precedence(temp) < precedence(Stack1.top()) ||
                           !Stack1.empty() && precedence(temp) == precedence(Stack1.top()) && associativity(str[i]) == 'L')
                    {
                        result += Stack1.top();
                        Stack1.pop();
                    }
                    Stack1.push(temp);
                }
            }

            while (!Stack1.empty())
            {
                result += Stack1.top();
                Stack1.pop();
            }
        }
        catch (...)
        {
            std::cerr << "Wrong Expression\n";
            result = "Wrong Expression";
        }
        return result;
    }

    static std::string Infix_to_prefix(std::string& str)
    {
        std::stack<char> Stack1;
        std::string output;
        std::ranges::reverse(str);
        uint32_t len = str.length();

        std::string temp = str;

        try
        {
            for (uint32_t i = 0; i < len; i++)
            {
                if (temp[i] == '(')
                    temp[i] = ')';

                else if (temp[i] == ')')
                    temp[i] = '(';
            }

            str = '(' + temp + ')';
            len = str.length();

            for (uint32_t i = 0; i < len; i++)
            {
                if (std::isalpha(str[i]) || std::isdigit(str[i]))
                    output += str[i];

                else if (str[i] == '(')
                    Stack1.push('(');

                else if (str[i] == ')')
                {
                    while (Stack1.top() != '(')
                    {
                        output += Stack1.top();
                        Stack1.pop();
                    }
                    Stack1.pop();
                }
                else
                {
                    if (isOperator(Stack1.top()))
                    {
                        if (str[i] == '^')
                        {
                            while (precedence(str[i]) <= precedence(Stack1.top()))
                            {
                                output += Stack1.top();
                                Stack1.pop();
                            }
                        }
                        else
                        {
                            while (precedence(str[i]) < precedence(Stack1.top()))
                            {
                                output += Stack1.top();
                                Stack1.pop();
                            }
                        }
                        Stack1.push(str[i]);
                    }
                }
            }
        } catch (...)
        {
            std::cerr << "Wrong Expression\n";
            return "Wrong Expression";
        }

        while (!Stack1.empty())
        {
            output += Stack1.top();
            Stack1.pop();
        }
        std::ranges::reverse(output);

        return output;
    }
};

int main()
{
    auto ui = HelloWorld::create();

    ui->on_process_textbox1_value([&]
    {
		std::string str = static_cast<std::string>(ui->get_my_textbox1());

           if (!str.empty())
           {
           	if (ExpressionEvaluate::isOperator(str[0]) && str[0] != '(')
                {
                    auto list = ExpressionEvaluate::Evaluate(str, 1);

                    if (str.length() == list.second.length())
                    {
                        ui->set_my_textbox2(static_cast<slint::SharedString>(list.first));
                        ui->set_my_textbox3(static_cast<slint::SharedString>(list.second));
                    }
                    else
                        goto first_block;
                }
                else
                {
                    first_block:
#ifdef _WIN32
				std::system("wscript run_hidden.vbs");
#else
				ui->set_my_textbox2("Wrong Expression");
				ui->set_my_textbox3("Wrong Expression");
#endif
                }
            }
	});

    ui->on_process_textbox2_value([&]
    {
		std::string str = static_cast<std::string>(ui->get_my_textbox2());

		if (!str.empty())
		{
		    if ((ExpressionEvaluate::isOperator(str[1]) && ExpressionEvaluate::isOperator(str[str.length() - 2])) || (str[0] == '(' && str[str.length() - 1] == ')'))
		    {
		        auto list = ExpressionEvaluate::Evaluate(str, 2);

		        if (list.first.length() == list.second.length())
		        {
		            ui->set_my_textbox1(static_cast<slint::SharedString>(list.first));
		            ui->set_my_textbox3(static_cast<slint::SharedString>(list.second));
		        }
		        else
		        	goto second_block;
		    }
		    else
		    {
		        second_block:
#if _WIN32
				std::system("wscript run_hidden.vbs");
#else
				ui->set_my_textbox1("Wrong Expression");
				ui->set_my_textbox3("Wrong Expression");
#endif
		    }
		}
	});

    ui->on_process_textbox3_value([&]
    {
            std::string str = static_cast<std::string>(ui->get_my_textbox3());

            if (!str.empty())
            {
                if (ExpressionEvaluate::isOperator(str[str.length() - 1]) && str[str.length() - 1] != ')')
                {
                    auto list = ExpressionEvaluate::Evaluate(str, 3);

                    if (str.length() == list.first.length())
                    {
                        ui->set_my_textbox1(static_cast<slint::SharedString>(list.first));
                        ui->set_my_textbox2(static_cast<slint::SharedString>(list.second));
                    }
                    else goto third_block;
                }
                else
                {
                    third_block:
#if _WIN32
                    std::system("wscript run_hidden.vbs");
#else
                    ui->set_my_textbox1("Wrong Expression");
                    ui->set_my_textbox2("Wrong Expression");
#endif
                }
            }
	});

    ui->run();
    return 0;
}
