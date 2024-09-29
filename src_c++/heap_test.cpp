
# include <iostream>
# include <vector>
# include <string>
# include <cassert>
# include <algorithm>
# include <map>
# include <set>
# include <utility>
# include <cmath>
using namespace std;

vector<pair<string, float> > create_heap(const vector<pair<string, float> >& heap_to_insert){
    
    // maintains a max heap
    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > heap;
    make_heap(heap.begin(), heap.end(), cmp); 
    int max_heap_size = 10;
    
    for (auto p: heap_to_insert){
        bool found = false;
        for (auto &token: heap){
            if (token.first == p.first){
                found = true;
                if (token.second > p.second){
                    token.second = p.second;
                }
                break;
            }
        }
        if (found){
            make_heap(heap.begin(), heap.end(), cmp);
        }
        else{
            if (heap.size() < max_heap_size){
                heap.push_back(p);

                if (heap.size() == max_heap_size){
                    make_heap(heap.begin(), heap.end(), cmp);
                }
            }
            
            else{
                // this is okay as we are using negative probability, if the heap one is larger than the new one, 
                // then replace it as smaller negative probability means bigger real probability
                if (heap.front().second > p.second){ 
                    pop_heap(heap.begin(), heap.end(), cmp); //  to move the top element of the heap to the end of the container
                    heap.pop_back(); // actually remove that element from the container

                    heap.push_back(p); // add the new element to the end of the container
                    push_heap(heap.begin(), heap.end(), cmp); // rearranges elements to make sure heap property is maintained
                }
                
            }
        }
        
    }

    return heap;
}

int main(){

    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > token_to_score;

    pair<string, float> p1("sel", -log(0.59585905));
    pair<string, float> p2("bng", -log(0.5354099));
    pair<string, float> p3("msg", -log(0.45632756));
    pair<string, float> p4("comport", -log(0.37669763));
    pair<string, float> p5("change", -log(0.65968746));
    pair<string, float> p6("floatatom", -log(0.58062696));
    pair<string, float> p7("*~", -log(0.6664535));
    pair<string, float> p8("dac~", -log(0.833791));
    pair<string, float> p9("*", -log(0.6053265));
    pair<string, float> p10("osc~", -log(0.5178137));
    pair<string, float> p11("list_prepend", -log(0.71278006));
    pair<string, float> p12("list_trim", -log(0.3476132));
    pair<string, float> p13("tabread", -log(0.57799923));
    pair<string, float> p14("msg" , -log(0.56799923));
    pair<string, float> p15("list_prepend", -log(0.59585905));
    pair<string, float> p16("list_trim", -log(0.65968746));
    pair<string, float> p17("tabread", -log(0.58062696));
    pair<string, float> p18("sel", -log(0.59585905));
    pair<string, float> p19("bng", -log(0.59585905));
    pair<string, float> p20("msg", -log(0.59585905));
    pair<string, float> p21("comport", -log(0.59585905));
    pair<string, float> p22("change", -log(0.59585905));
    pair<string, float> p23("floatatom", -log(0.59585905));
    pair<string, float> p24("*~", -log(0.59585905));
    pair<string, float> p25("dac~", -log(0.59585905));
    pair<string, float> p26("*", -log(0.59585905));
    pair<string, float> p27("osc~", -log(0.59585905));
    pair<string, float> p28("list_prepend", -log(0.59585905));
    pair<string, float> p29("list_trim", -log(0.59585905));
    pair<string, float> p30("tabread", -log(0.59585905));


    token_to_score.push_back(p1);
    token_to_score.push_back(p2);
    token_to_score.push_back(p3);
    token_to_score.push_back(p4);
    token_to_score.push_back(p5);
    token_to_score.push_back(p6);
    token_to_score.push_back(p7);
    token_to_score.push_back(p8);
    token_to_score.push_back(p9);
    token_to_score.push_back(p10);
    token_to_score.push_back(p11);
    token_to_score.push_back(p12);
    token_to_score.push_back(p13);

    vector<pair<string, float> > heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

   
    vector<pair<string, float> > correct_heap;

    correct_heap.push_back(p8);
    correct_heap.push_back(p11);
    correct_heap.push_back(p7);
    correct_heap.push_back(p5);
    correct_heap.push_back(p9);
    correct_heap.push_back(p1);
    correct_heap.push_back(p6);
    correct_heap.push_back(p13);
    correct_heap.push_back(p2);
    correct_heap.push_back(p10);
    
    // test case 1
    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }

    cout << "Test case 1 passed with more than max_heap_size elements\n";
    
    
    // test case 2
    token_to_score.push_back(p14);
    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    vector<pair<string, float> > correct_heap_2;

    pair<string, float> correct_p11("msg", -log(0.56799923));
    correct_heap.clear();

    correct_heap.push_back(p8);
    correct_heap.push_back(p11);
    correct_heap.push_back(p7);
    correct_heap.push_back(p5);
    correct_heap.push_back(p9);
    correct_heap.push_back(p1);
    correct_heap.push_back(p6);
    correct_heap.push_back(p13);
    correct_heap.push_back(p14);
    correct_heap.push_back(p2);


    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }

    cout << "Test case 2 passed with more than max_heap_size elements and repeated tokens with different probabilities\n";
    
    // test case 3
    token_to_score.clear();
    token_to_score.push_back(p1);
    token_to_score.push_back(p7);
    token_to_score.push_back(p6);
    token_to_score.push_back(p4);

    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    correct_heap.clear();

    correct_heap.push_back(p7);
    correct_heap.push_back(p1);
    correct_heap.push_back(p6);
    correct_heap.push_back(p4);

    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }
    cout << "Test case 3 passed with fewer than max_heap_size elements\n";

    // test case 4
    token_to_score.clear();
    token_to_score.push_back(p1);
    token_to_score.push_back(p7);
    token_to_score.push_back(p6);
    token_to_score.push_back(p4);
    token_to_score.push_back(p14);
    token_to_score.push_back(p3);

    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    correct_heap.clear();

    correct_heap.push_back(p7);
    correct_heap.push_back(p1);
    correct_heap.push_back(p6);
    correct_heap.push_back(p14);
    correct_heap.push_back(p4);

    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }

    cout << "Test case 4 passed with fewer than max_heap_size elements and repeated tokens with different probabilities\n";


    // test case 5
    token_to_score.clear();
    token_to_score.push_back(p1);
    token_to_score.push_back(p2);
    token_to_score.push_back(p3);
    token_to_score.push_back(p4);
    token_to_score.push_back(p5);
    token_to_score.push_back(p6);
    token_to_score.push_back(p7);
    token_to_score.push_back(p8);
    token_to_score.push_back(p9);
    token_to_score.push_back(p10);

    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    correct_heap.clear();

    correct_heap.push_back(p8);
    correct_heap.push_back(p7);
    correct_heap.push_back(p5);
    correct_heap.push_back(p9);
    correct_heap.push_back(p1);
    correct_heap.push_back(p6);
    correct_heap.push_back(p2);
    correct_heap.push_back(p10);
    correct_heap.push_back(p3);
    correct_heap.push_back(p4);

    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }

    cout << "Test case 5 passed with exactly max_heap_size elements\n";


    // test case 6

    token_to_score.clear();
    token_to_score.push_back(p1);
    token_to_score.push_back(p2);
    token_to_score.push_back(p3);
    token_to_score.push_back(p4);
    token_to_score.push_back(p5);
    token_to_score.push_back(p6);
    token_to_score.push_back(p7);
    token_to_score.push_back(p8);
    token_to_score.push_back(p9);
    token_to_score.push_back(p10);
    token_to_score.push_back(p14);
    token_to_score.push_back(p15);
    token_to_score.push_back(p16);
    token_to_score.push_back(p17);

    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    correct_heap.clear();

    correct_heap.push_back(p8);
    correct_heap.push_back(p7);
    correct_heap.push_back(p5);
    correct_heap.push_back(p16);
    correct_heap.push_back(p9);
    correct_heap.push_back(p15);
    correct_heap.push_back(p1);
    correct_heap.push_back(p17);
    correct_heap.push_back(p6);
    correct_heap.push_back(p14);

    for (int i = 0; i < heap.size(); i++){
        assert(heap[i].first == correct_heap[i].first && heap[i].second == correct_heap[i].second);
    }

    cout << "Test case 6 passed with more than max_heap_size elements and same probabilities\n";

    // test case 7

    token_to_score.clear();
    token_to_score.push_back(p18);
    token_to_score.push_back(p19);
    token_to_score.push_back(p20);
    token_to_score.push_back(p21);
    token_to_score.push_back(p22);
    token_to_score.push_back(p23);
    token_to_score.push_back(p24);
    token_to_score.push_back(p25);
    token_to_score.push_back(p26);
    token_to_score.push_back(p27);
    token_to_score.push_back(p28);
    token_to_score.push_back(p29);
    token_to_score.push_back(p30);

    heap = create_heap(token_to_score);
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp);

    cout << "Test case 7 with more than max_heap_size elements and same probabilities, nothing to assert, the first 10 elements get inserted in the heap\n";

    cout << "All tests passed\n";




    return 0;
}