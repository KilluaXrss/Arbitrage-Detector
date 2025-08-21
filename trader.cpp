#include "receiver.h"


#include <vector>
#include <string>
class TwoVectorStructure {
public:
    std::vector<std::pair<std::string ,int>> vector1;
    std::vector<int> vector2;  
};
class ThreeVectorStructure {
public:
    std::vector<std::pair<std::string ,int>> vector1;
    std::vector<int> vector2;
    std::vector<int> vector3;
};
int search(std::string stock_name , std::vector<std::pair<std:: string ,int>> st){
    int n = st.size();
    for(int i =0;i<n;i++){
        if(st[i].first == stock_name){
            return i;
        }
    }
    return -1;
}
void add_vectors(std::vector<std::pair<std::string,int>>&vector_1,std::vector<std::pair<std::string,int>>vector_2){
    int n_2 = vector_2.size();
    for(int i =0;i<n_2;i++){
        vector_1[i].second+=vector_2[i].second;
    }
}
void add_vectors_part3(std::vector<std::pair<std::string,int>>&vector_1,std::vector<std::pair<std::string,int>>vector_2,int times){
    int n_2 = vector_2.size();
    for(int i =0;i<n_2;i++){
        vector_1[i].second+=times*vector_2[i].second;
    }
}
void multiply(std::vector<std::pair<std::string,int>>&vector_1,int times){
    int n = vector_1.size();
    for(int i =0;i<n;i++){
        vector_1[i].second*=times;
    }
}
bool cycle_found(std::vector<std::pair<std::string,int>>vector_cycle){
    int n = vector_cycle.size();
    for(int i =0;i<n;i++){
        if(vector_cycle[i].second == 0){
            continue;
        }
        else{
            return false;
        }
    }
    if(n ==0){
        return false;
    }
    return true;
}
void recurssion(TwoVectorStructure temp_struct,std::vector<std::vector<std::pair<std::string,int>>>matrix,int i,std::vector<std::vector<int>>&cycle_matrix,std::vector<bool>cancellation_check){
    if(i == 0){
        return;
    }
    TwoVectorStructure sum_struct = temp_struct;
    if(cancellation_check[i-1]){
    add_vectors(sum_struct.vector1,matrix[i-1]);
    sum_struct.vector2.push_back(i-1);
    bool check_cycle = cycle_found(sum_struct.vector1);
    if(check_cycle){ 
        cycle_matrix.push_back(sum_struct.vector2);
    } 
    recurssion(sum_struct,matrix,i-1,cycle_matrix,cancellation_check);
    }
    recurssion(temp_struct,matrix,i-1,cycle_matrix,cancellation_check);
}
void recurssion_part3(ThreeVectorStructure temp_struct,std::vector<std::vector<std::pair<std::string,int>>>matrix,int i,std::vector<std::vector<int>>&cycle_matrix,std::vector<std::vector<int>>&quantity_matrix,std::vector<int>quantities){
    if(i == 0){
        return;
    }
    int q = quantities[i-1];
    for(int j =0;j<q;j++){  
        ThreeVectorStructure sum_struct = temp_struct;
        add_vectors_part3(sum_struct.vector1,matrix[i-1],j+1);
        sum_struct.vector2.push_back(i-1);
        sum_struct.vector3.push_back(j+1);
        bool check_cycle = cycle_found(sum_struct.vector1);
        if(check_cycle){   
            cycle_matrix.push_back(sum_struct.vector2);
            quantity_matrix.push_back(sum_struct.vector3);
        }
        recurssion_part3(sum_struct,matrix,i-1,cycle_matrix,quantity_matrix,quantities);
    }
    recurssion_part3(temp_struct,matrix,i-1,cycle_matrix,quantity_matrix,quantities);
}
void process(std::string stock_name ,int price,std::string buy_sell,std::vector<std::pair<std::string ,int>>&ptr,std::vector<std::pair<std::string ,int>>&cancel_s,std::vector<std::pair<std::string ,int>>&cancel_b){
    std::string out_buy_sell;
    if(buy_sell =="s" ){
        out_buy_sell = "b";
    }
    else if(buy_sell =="b" ){
        out_buy_sell = "s";
    }
    if(ptr.size()==0){
        ptr.push_back({stock_name,price});
        std::cout<<stock_name<<' '<<price<<' '<<out_buy_sell<<std::endl;
        return;
    }
    else{
        int j = search(stock_name,ptr);
        if(j == -1){
        ptr.push_back({stock_name,price});
        std::cout<<stock_name<<' '<<price<<' '<<out_buy_sell<<std::endl;
        return;  
        }
        else{
            if(buy_sell == "s"){
                if(cancel_s.size()!=0){
                    int k = search(stock_name,cancel_s);
                    if(k!=-1){
                        if(price<cancel_s[k].second){
                            cancel_s[k].second = price;
                        }
                        else if(price >= cancel_s[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            return;
                        }
                    }
                }
                if(cancel_b.size()!=0){
                    int k = search(stock_name,cancel_b);
                    int x = search(stock_name,cancel_s);
                    if(k!=-1&& x==-1){
                        if(price == cancel_b[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            cancel_b[k].first = "@";
                            return;
                        }
                    }
                    if(k!=-1 && x!=-1){
                        if(cancel_s[x].second == cancel_b[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            cancel_b[k].first = "@";
                            cancel_s[x].first = "@";
                            return;
                        }
                    }
                }
                if(price<ptr[j].second){
                    int x = search(stock_name,cancel_s);
                    std::cout<<stock_name<<' '<<price<<' '<<out_buy_sell<<std::endl;
                    ptr[j].second = price;
                    if(x == -1){
                        return;
                    }
                    if(price == cancel_s[x].second){
                        cancel_s[x].first = "@";
                    }
                    return;
                }
                else{
                    std::cout<<"No Trade"<<std::endl;
                    if(cancel_s.size()==0){
                        cancel_s.push_back({stock_name,price});
                        return;
                    }
                    else{
                        int h = search(stock_name,cancel_s);
                        if(h == -1){
                        cancel_s.push_back({stock_name,price});
                        return;
                        }
                        else{
                            if(price<cancel_s[h].second){
                                cancel_s[h].second = price;
                                return;
                            }
                            else{
                                return;
                            }
                        }
                    }
                }
            }
            else if(buy_sell == "b"){
                if(cancel_b.size()!=0){
                    int k = search(stock_name,cancel_b);
                    if(k!=-1){
                        if(price>cancel_b[k].second){
                            cancel_b[k].second = price;
                        }
                        else if(price <= cancel_b[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            return;
                        }
                    }
                }
                if(cancel_s.size()!=0){
                    int k = search(stock_name,cancel_s);
                    int x = search(stock_name,cancel_b);
                    if(k!=-1&& x==-1){
                        if(price == cancel_s[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            cancel_s[k].first = "@";
                            return;
                        }
                    }
                    if(k!=-1 && x!=-1){
                        if(cancel_b[x].second == cancel_s[k].second){
                            std::cout<<"No Trade"<<std::endl;
                            cancel_s[k].first = "@";
                            cancel_b[x].first = "@";
                            return;
                        }
                    }
                }
                if(price>ptr[j].second){
                    std::cout<<stock_name<<' '<<price<<' '<<out_buy_sell<<std::endl;
                    int x = search(stock_name,cancel_b);
                    ptr[j].second = price;
                    if(x == -1){
                        return;
                    }
                    if(price == cancel_b[x].second){
                        cancel_b[x].first = "@";
                    }
                    return;
                }
                else{
                    std::cout<<"No Trade"<<std::endl;
                    if(cancel_b.size()==0){
                        cancel_b.push_back({stock_name,price});
                        return;
                    }
                    else{
                        int h = search(stock_name,cancel_b);
                        if(h == -1){
                        cancel_b.push_back({stock_name,price});
                        return;
                        }
                        else{
                            if(price>cancel_b[h].second){
                                cancel_b[h].second = price;
                                return;
                            }
                            else{
                                return;
                            }
                        }
                    }
                }               
            }
        }
    }
}
bool compare_vectors(std::vector<std::pair<std::string,int>>vector_1,std::vector<std::pair<std::string,int>>vector_2,char buy_sell_1,char buy_sell_2){
    if(vector_1.size()!=vector_2.size()){
        return false;
    }
    int n = vector_1.size();
    if(buy_sell_1 == buy_sell_2){
        for(int i =0;i<n;i++){

                if(vector_1[i].second != vector_2[i].second){
                    return false;
                }
            
        }
    }
    else{
        for(int i =0;i<n;i++){
                if(vector_1[i].second != -vector_2[i].second){
                    return false;
                }
            
        }
    }
    return true;
}
void cancellation_part3(std::vector<std::pair<std::string ,int>>temp_vector,std::vector<std::vector<std::pair<std::string,int>>>matrix,std::vector<int>&quantites,std::vector<char>sell_buy,int i,std::vector<int>prices){
    bool compare;
    if(sell_buy[i]=='b'){
        for(int j = i-1;j>=0;j--){
            if(sell_buy[j] == 's'){
                compare= compare_vectors(temp_vector,matrix[j],sell_buy[i],sell_buy[j]);
                if(compare && prices[i]==-prices[j]){
                    if(quantites[i]>quantites[j]){
                        quantites[i]-=quantites[j];
                        quantites[j]=0;
                        
                    }
                    else{
                        quantites[j]-=quantites[i];
                        quantites[i]=0;
                        
                    }
                }
            }
            else{
                compare= compare_vectors(temp_vector,matrix[j],sell_buy[i],sell_buy[j]);
                if(compare && prices[i]==prices[j]){
                    quantites[i]+=quantites[j];
                    quantites[j] = 0;
                }
            }
        }
    }
    else if(sell_buy[i]=='s'){
       for(int j = i-1;j>=0;j--){
            if(sell_buy[j] == 'b'){
                compare= compare_vectors(temp_vector,matrix[j],sell_buy[i],sell_buy[j]);
                if(compare && prices[i]==-prices[j]){
                    if(quantites[i]>quantites[j]){
                        quantites[i]-=quantites[j];
                        quantites[j]=0;
                        
                        
                    }
                    else{
                        
                        quantites[j]-=quantites[i];
                        quantites[i]=0;
                       
                    }
                }
            }
            else{
                compare= compare_vectors(temp_vector,matrix[j],sell_buy[i],sell_buy[j]);
                if(compare && prices[i]==prices[j]){
                    quantites[i]+=quantites[j];
                    quantites[j] = 0;
                }
            }
        } 
    }
}
void cancellation(std::vector<std::pair<std::string,int>>temp_vector,std::vector<std::vector<std::pair<std::string,int>>>&matrix,std::vector<char>buy_sell,std::vector<int>prices,int i, std::vector<bool>&cancellation_check){
    std::vector<std::pair<std::string,int>>buff_vector;
    bool compare;
    if(buy_sell[i]== 'b'){
        for(int j = i-1;j>=0;j--){
            if(buy_sell[j]=='b'){
                compare= compare_vectors(temp_vector,matrix[j],buy_sell[i],buy_sell[j]);
                if(!compare || !cancellation_check[j]){
                    continue;
                }
                else{
                    if(prices[i]>prices[j]){
                        
                        cancellation_check[j] = false;
                        break;
                    }
                    else{
                        
                        cancellation_check[i] = false;
                        temp_vector = matrix[j];
                        break;
                    }
                }
            }
        }
        for(int j = i-1;j>=0;j--){
            if(buy_sell[j]=='s'){
                compare= compare_vectors(temp_vector,matrix[j],buy_sell[i],buy_sell[j]);
                if(!compare || !cancellation_check[j]){
                    continue;
                }
                else{
                    if(prices[i]==-prices[j]){
                        
                        cancellation_check[i]= false;
                        cancellation_check[j]= false;

                        break;
                    }
                }
            }
        }
    }
    else{
        for(int j = i-1;j>=0;j--){
            if(buy_sell[j]=='s'){
                compare= compare_vectors(temp_vector,matrix[j],buy_sell[i],buy_sell[j]);
                if(!compare || !cancellation_check[j]){
                    continue;
                }
                else{
                    if(prices[i]>prices[j]){
                        
                        cancellation_check[j] = false;
                        break;
                    }
                    else{
                        
                        temp_vector = matrix[j];
                        cancellation_check[i] = false;
                        break;
                    }
                }
            }
        }
        for(int j = i-1;j>=0;j--){
            if(buy_sell[j]=='b'){
                compare= compare_vectors(temp_vector,matrix[j],buy_sell[i],buy_sell[j]);
                if(!compare || !cancellation_check[j]){
                    continue;
                }
                else{
                    if(prices[i]==-prices[j]){
                        
                        cancellation_check[i] = false;
                        cancellation_check[j] = false;
                        break;
                    }
                }
            }
        }
    }
}
int main(int argc,char** argv) {
    Receiver rcv;
    // sleep(5);
    bool found_dollar = false;
    std::string message = rcv.readIML();
    while(!found_dollar){
        std::string message1 = rcv.readIML();
        message.append(message1);
        if(message.find('$') != std::string::npos){
            //std::message1 = rcv.readIML();
            found_dollar= true;
        }
    }
    char suhas = argv[1][0];
    if(suhas == '1'){
        std::string stock_name;
        std::string stock_price;
        std::string buy_sell;
        size_t start = 0;
        size_t end = 0;
        std::vector<std::pair<std::string,int>>st;
        std::vector<std::pair<std::string,int>>cancellation_s;
        std::vector<std::pair<std::string,int>>cancellation_b;
        while(message[end+1]!='$'){
        end = message.find(' ', start);
        if (end != std::string::npos) {
            stock_name = message.substr(start, end - start);
            start = end + 1;
        }
        end = message.find(' ', start);
        if (end != std::string::npos) {
            stock_price = message.substr(start, end - start);
            start = end + 1;
        }
        int price = std::stoi(stock_price);   
        end = message.find('#', start);
        if (end != std::string::npos) {
            buy_sell = message.substr(start, end - start);
        }
        process(stock_name,price,buy_sell,st,cancellation_s,cancellation_b);
        start = end+2;
        }
    }
    else if(suhas == '2'){
        std::vector<std::vector<std::pair<std::string, int>>> matrix;
        std::vector<std::vector<std::pair<std::string, int>>> matrix1;
        std::vector<std::pair<std::string,int>>buff_vector;
        buff_vector.push_back({"@",1});
        std::vector<bool>cancellation_check;
        int quantity;        
        std::vector<int>prices;
        std::vector<std::pair<std::string ,int>>stox;
        std::vector<char>sell_buy;
        std::vector<char>out_sell_buy;
        std::vector<int>profit;
        int message_i = 0;
        int stock_j = 0;
        int k = 0;
        int line_num = 0;
        while(message[message_i]!= '$'){
            while(message[message_i]!='#'){
                message_i++;
            }
            stock_j = message_i-3;
            while(message[stock_j] != ' '){
                stock_j--;
            }
            std::vector<std::pair<std::string ,int>>temp_vector;
            std::vector<std::pair<std::string ,int>>temporary_vector;
            bool check = true;
            while(k <stock_j){
                std::string stock_name;
                std:: string stock_quantity;
                while(message[k]!= ' '){
                    stock_name+=message[k];
                    k++;
                }
                int m = search(stock_name,stox);
                if(m == -1){
                    stox.push_back({stock_name,2147483647});
                    check = false;
                    if(matrix1.size()!=0){
                        int matsize = matrix1.size();
                        for(int varu = matsize-1;varu>=0;varu--){
                            matrix1[varu].push_back({stock_name,0});
                        }
                    }
                }
                k++;
                while(message[k]!=' '){
                    stock_quantity+=message[k];
                    k++;
                }
                quantity = std::stoi(stock_quantity);

                if(message[message_i-1]=='s'){
                    quantity = -quantity;
                }
                k++;
                temp_vector.push_back({stock_name,quantity});
            }
            sell_buy.push_back(message[message_i-1]);
            if(message[message_i-1]=='s'){
                out_sell_buy.push_back('b');
            }
            else if(message[message_i-1]=='b'){
                out_sell_buy.push_back('s');
            }
            k = message_i+2;
            message_i++;
            int stox_size = stox.size();
            for(int suh = 0; suh<stox_size; suh++){
                int putx = search(stox[suh].first,temp_vector);
                if(putx == -1){
                    temporary_vector.push_back({stox[suh].first,0});
                }
                else{
                    temporary_vector.push_back({stox[suh].first,temp_vector[putx].second});
                }
            }
            matrix1.push_back(temporary_vector);
            matrix.push_back(temp_vector);
            cancellation_check.push_back(true);
            std::vector<int>cycle;
            std::vector<std::vector<int>>cycle_matrix;
            line_num++;
            TwoVectorStructure mystruct;
            
            
            mystruct.vector2.push_back(line_num-1);
            std:: string total_price;
            while(message[stock_j+1]!=' '){
                total_price+=message[stock_j+1];
                stock_j++;
            }
            if(sell_buy[line_num-1]== 's'){
            prices.push_back(-std::stoi(total_price));
            }
            else{
                prices.push_back(std::stoi(total_price));
            }

            mystruct.vector1 = matrix1[line_num-1];
            cancellation(temporary_vector,matrix1,sell_buy,prices,line_num-1,cancellation_check);
            if(cancellation_check[line_num-1] && check){
            recurssion(mystruct,matrix1,line_num-1,cycle_matrix,cancellation_check); 
            }
            if(cycle_matrix.size()==0){
                std::cout<<"No Trade"<<std::endl;
            }
            if(cycle_matrix.size()!=0){
                int n = cycle_matrix.size();
                std::vector<int> sum_of_prizes;
                for(int i =0;i<n;i++){
                    int size = cycle_matrix[i].size();
                    sum_of_prizes.push_back(0);
                    for(int j =0;j<size;j++){
                        sum_of_prizes[i]+=prices[cycle_matrix[i][j]];
                    }
                }
                int j = 0;
                for(int i =0;i<n;i++){
                    if(sum_of_prizes[j]<sum_of_prizes[i]){
                        j = i;
                    }
                }
                if(sum_of_prizes[j]<=0){
                    std::cout<<"No Trade"<<std::endl;
                }
                else{
                    n = cycle_matrix[j].size();
                    for(int i =0;i<n;i++){
                        int x = matrix[cycle_matrix[j][i]].size();
                        if(sell_buy[cycle_matrix[j][i]]=='b'){
                            for(int h =0;h<x;h++){
                                std::cout<<matrix[cycle_matrix[j][i]][h].first<<' '<<matrix[cycle_matrix[j][i]][h].second<<' ';
                            }
                        }
                        else if(sell_buy[cycle_matrix[j][i]]=='s'){
                        for(int h =0;h<x;h++){
                                std::cout<<matrix[cycle_matrix[j][i]][h].first<<' '<<-matrix[cycle_matrix[j][i]][h].second<<' ';
                            } 
                        }
                        cancellation_check[cycle_matrix[j][i]] = false;
                        if(sell_buy[cycle_matrix[j][i]]=='b'){
                        std::cout<<prices[cycle_matrix[j][i]]<<' '<<out_sell_buy[cycle_matrix[j][i]]<<std::endl;
                        }
                        else{
                            std::cout<<-prices[cycle_matrix[j][i]]<<' '<<out_sell_buy[cycle_matrix[j][i]]<<std::endl;
                        }
                    }
                    profit.push_back(sum_of_prizes[j]);
                }
            }
        }
        int n = profit.size();
        int net_profit = 0;
        for(int i =0;i<n;i++){
            net_profit+=profit[i];
        }
        std::cout<<net_profit;
    }
    else if(suhas == '3'){
        std::vector<std::vector<std::pair<std::string, int>>> matrix;
        std::vector<std::vector<std::pair<std::string, int>>> matter;
        std::vector<std::vector<int>>number_of_times;
        int quantity;     
        std::vector<std::pair<std::string ,int>>tox;   
        std::vector<int>prices;
        std::vector<char>sell_buy;
        std::vector<char>out_sell_buy;
        std::vector<int>quantities;
        std::vector<int>final_quantities;
        int net_price = 0;
        // size_t start =0;
        // size_t end =0;
        int message_i = 0;
        int stock_j = 0;
        int k = 0;
        int line_num = 0;
        int quantity_j = 0;
        while(message[message_i]!= '$'){
            while(message[message_i]!='#'){
                message_i++;
            }
            stock_j = message_i-3;
            while(message[stock_j]!=' '){
                stock_j--;
            }
            std::string net_quantity;
            quantity_j = stock_j+1;
            stock_j--;
            while ((message[quantity_j])!=' ')
            {
                net_quantity+=message[quantity_j];
                quantity_j++;
            }
            quantities.push_back(std::stoi(net_quantity));
             
            while(message[stock_j] != ' '){
                stock_j--;
            }
            std::string price_per_order;
            quantity_j = stock_j+1;
            while ((message[quantity_j])!=' ')
            {
                price_per_order+=message[quantity_j];
                quantity_j++;
            }
            if(message[message_i-1]=='b'){
                prices.push_back(std::stoi(price_per_order));
            }
            else if(message[message_i-1]=='s'){
                prices.push_back(-std::stoi(price_per_order));
            }
            std::vector<std::pair<std::string ,int>>temp_vector;
            std::vector<std::pair<std::string ,int>>tempo_vector;
            bool check = true;
            while(k <stock_j){
                std::string stock_name;
                std:: string stock_quantity;
                while(message[k]!= ' '){
                    stock_name+=message[k];
                    k++;
                }
                int m = search(stock_name,tox);
                if(m == -1){
                    tox.push_back({stock_name,2147483647});
                    check = false;
                    if(matter.size()!=0){
                        int matsize = matter.size();
                        for(int varu = matsize-1;varu>=0;varu--){
                            matter[varu].push_back({stock_name,0});
                        }
                    }
                }
                k++;
                while(message[k]!=' '){
                    stock_quantity+=message[k];
                    k++;
                }
                quantity = std::stoi(stock_quantity);

                if(message[message_i-1]=='s'){
                    
                    quantity = -quantity;
                    
                }
                k++;
                temp_vector.push_back({stock_name,quantity});
            }
            sell_buy.push_back(message[message_i-1]);
            if(message[message_i-1]=='s'){
                out_sell_buy.push_back('b');
            }
            else if(message[message_i-1]=='b'){
                out_sell_buy.push_back('s');
            }
            k = message_i+2;
            message_i++;
            int stox_size = tox.size();
            for(int suh = 0; suh<stox_size; suh++){
                int putx = search(tox[suh].first,temp_vector);
                if(putx == -1){
                    tempo_vector.push_back({tox[suh].first,0});
                }
                else{
                    tempo_vector.push_back({tox[suh].first,temp_vector[putx].second});
                }
            }
            matter.push_back(tempo_vector);
            matrix.push_back(temp_vector);
            line_num++;
            std::vector<std::vector<int>>cycle_matrix;
            std::vector<std::vector<int>>quantity_matrix;
            // std::cout<<"123";
            
            cancellation_part3(tempo_vector,matter,quantities,sell_buy,line_num-1,prices);
            int q_n = quantities[quantities.size()-1];
            for(int i =0;i<q_n;i++){
                ThreeVectorStructure mystruct;
                mystruct.vector2.push_back(line_num-1);
                mystruct.vector1 = tempo_vector;
                mystruct.vector3.push_back(i+1);
                multiply(mystruct.vector1,i+1);
                // std::cout<<"hi";
                if(check){
                recurssion_part3(mystruct,matter,line_num-1,cycle_matrix,quantity_matrix,quantities); 
                }
                // std::cout<<"bye";
            }
            if(cycle_matrix.size()==0){
                std::cout<<"No Trade"<<std::endl;
            }
            if(cycle_matrix.size()!=0){
                int n = cycle_matrix.size();
                std::vector<int> sum_of_prizes;
                for(int i =0;i<n;i++){
                    int size = cycle_matrix[i].size();
                    sum_of_prizes.push_back(0);
                    for(int j =0;j<size;j++){
                        sum_of_prizes[i]+=prices[cycle_matrix[i][j]]*quantity_matrix[i][j];
                    }
                }
                int j = 0;
                for(int i =0;i<n;i++){
                    if(sum_of_prizes[j]<sum_of_prizes[i]){
                        j = i;
                    }
                }
                // std::cout<<"suhas";
                if(sum_of_prizes[j]<=0){
                    std::cout<<"No Trade"<<std::endl;
                }
                
                if(sum_of_prizes[j]>0)
                {   net_price+=sum_of_prizes[j];
                    n = cycle_matrix[j].size();
                    for(int i =0;i<n;i++){
                        int x = matrix[cycle_matrix[j][i]].size();
                        if(sell_buy[cycle_matrix[j][i]]=='b'){
                            for(int h =0;h<x;h++){
                                std::cout<<matrix[cycle_matrix[j][i]][h].first<<' '<<matrix[cycle_matrix[j][i]][h].second<<' ';
                            }
                            
                        }
                        else if(sell_buy[cycle_matrix[j][i]]=='s'){
                            for(int h =0;h<x;h++){
                                std::cout<<matrix[cycle_matrix[j][i]][h].first<<' '<<-matrix[cycle_matrix[j][i]][h].second<<' ';
                            } 
                            
                        }
                        if(sell_buy[cycle_matrix[j][i]] == 'b'){
                            std::cout<<prices[cycle_matrix[j][i]]<<" "<<quantity_matrix[j][i]<<" "<<out_sell_buy[cycle_matrix[j][i]]<<std::endl;
                        }
                        else{
                            std::cout<<-prices[cycle_matrix[j][i]]<<" "<<quantity_matrix[j][i]<<" "<<out_sell_buy[cycle_matrix[j][i]]<<std::endl;
                        }
                        quantities[cycle_matrix[j][i]]-=quantity_matrix[j][i];


                }
                
            }

    }

}
    std::cout<<net_price;


    }
    rcv.terminate();
    return 0;
}