from ast import literal_eval
#USART Helper

def __Convert_USART_Float_To_Int(USART_DIV : float) -> int:
    Fraction : int = (int(USART_DIV) & 0xFF) << 4
    Mantissa : int = (int(round(16 * USART_DIV) & 0xF)) << 0
    return (Fraction | Mantissa)

def USART_Calculate_USARTDIV(USART_Fck : int, USART_Baud : int) -> float:
    """Uses the USART_Fck and USART_Baud to calculate the USARTDIV 

    Args:
        USART_Fck (int): USART Prepheral Frequency
        USART_Baud (int): Desired Baud Rate

    Returns:
        float: USART Divider
    """
    return USART_Fck / (USART_Baud * 16)

def USART_Calculate_Fraction_And_Mantissa(USART_Div : float) -> tuple[int, int]:
    """Uses the USART_Div to calculate the Fraction and Mantissa Value

    Args:
        USART_Div (float): USART Divider

    Returns:
        tuple[int, int]: Fraction, Mantissa
    """
    Div : int = __Convert_USART_Float_To_Int(USART_Div)
    Fraction : int = (Div >> 4) & 0xFF
    Mantissa : int = (Div >> 0) & 0x0F
    return (Fraction, Mantissa)

def USART_Calculate_USARTDIV_From_BRR(USART_BRR: int) -> float:
    """Uses the value in the BRR to calculate the USARTDIV

    Args:
        USART_BRR (int): USART Baud Rate Register

    Returns:
        float: USART Divider
    """
    Fraction = (USART_BRR >> 4) & 0xFF
    Mantissa = (USART_BRR >> 0) & 0x0F
    return Fraction + Mantissa/16

def USART_Calculate_Baud_From_Fck_And_USARTDIV(USART_Fck : int, USARTDIV : float) -> int:
    return int(USART_Fck / (16 * USARTDIV))

def main():
    value = input("\n1. Calculate USARTDIV from Fck and Baud\n" +
                    "2. Calculate Fraction and Mantissa from USARTDIV\n" +
                    "3. Calculate USARTDIV from BRR\n" +
                    "4. Calculate Baud from Fck and USARTDIV\n" +
                    "Please Select an option: ")
    
    UsartDiv : float 
    
    USART_Clk_Freq : int
    DesiredBaud : int
    Fraction : int
    Mantissa : int
    BRR : int
    
    match value:
        case "1":
            DesiredBaud = int(input("\tPlease enter the Desired Baud Rate: "))
            USART_Clk_Freq = int(input("\tPlease enter the Clk Frequency of the USART Prepheral: "))
            
            UsartDiv = USART_Calculate_USARTDIV(USART_Clk_Freq, DesiredBaud)
            print(F"\t\tUSARTDIV: {UsartDiv}")
        case "2":
            UsartDiv = float(input("\tPlease enter the USARTDIV: "))
            
            Fraction, Mantissa = USART_Calculate_Fraction_And_Mantissa(UsartDiv)
            print(F"\t\tFraction: {Fraction}\n\t\tMantissa: {Mantissa}")
        case "3":
            userIn = input("\tPlease enter the BRR value: ")
            if(userIn.startswith("0x") or userIn.startswith("0b")):
                BRR = literal_eval(userIn)
            else:
                BRR = int(userIn)
            
            UsartDiv = USART_Calculate_USARTDIV_From_BRR(BRR)
            print(F"\tUSARTDIV: {UsartDiv}")
        case "4":
            USART_Clk_Freq = int(input("\tPlease enter the Clk Frequency of the USART Prepheral: "))
            UsartDiv = float(input("\tPlease enter the USARTDIV: "))
            
            DesiredBaud = USART_Calculate_Baud_From_Fck_And_USARTDIV(USART_Clk_Freq, UsartDiv)
            print(F"\t\tBaud: {DesiredBaud}")
        case _:
            print("You have entered in an incorrect value!") 
    
main()