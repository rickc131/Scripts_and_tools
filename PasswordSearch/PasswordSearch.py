import sys
import requests
from bs4 import BeautifulSoup


'''
----------------------------- 
use:
 python PasswordSearch.py 大华
-----------------------------
'''


def get_sth(na, nu):
    res = requests.get(
        url="https://mima.anhu.cc/index.php?keyword=" + na + "&page=" + nu,
        headers={"Host": "mima.anhu.cc"}
    )
    return res


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print("Please enter keyword")
        sys.exit()
    else:
        for i in range(1, 20):
            name = sys.argv[1]
            soup = BeautifulSoup(get_sth(name, str(i)).text, 'html.parser')
            table = soup.find('table')
            if table is not None:
                headers = [th.get_text(strip=True) for th in table.thead.find_all('th')]

                # 提取表格数据
                rows = []
                for tr in table.tbody.find_all('tr'):
                    rows.append([td.get_text(strip=True) for td in tr.find_all('td')])

                # 计算每列最大宽度
                max_widths = []
                for col_idx in range(len(headers)):
                    column_data = [row[col_idx] for row in rows]
                    max_widths.append(max(len(item) for item in column_data))

                # 构建对齐格式
                format_str = "".join([f" {{:<{width}}} " for width in max_widths])

                # 打印数据行
                for row in rows:
                    print(format_str.format(*row))
            else:
                break
