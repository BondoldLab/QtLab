
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtGlobal>
#include <QTime>
#include <QtGui>
#include <iostream>

QT_CHARTS_USE_NAMESPACE


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QList<double> dataList;
    qsrand( QTime::currentTime().msec() );

    //えせ正規分布　今回はここが重要ではないので省略。
    for(int i=0;i<200;i++){
        double rand = qrand()%100;
        rand = rand /1000;
        //rand = rand /1000;// *-1;
//        if(rand > 50){
//            rand -= qrand()%20;
//        }else{
//            rand += qrand()%20;
//        }
        dataList.append(rand);
    }

    //昇順ソート
    qSort(dataList);

    //最小値と最大値の取得
    double min = dataList[0];
    double max = dataList[dataList.count()-1];

    //桁数のまるめ。これは実験だからここに書くが、ここらへんは関数にするべき。
    int iLog;
    int iPow;
    //x軸最大値の算出
    //テストは必ず正の値、負の値、＋１未満-1超えの小数点で行う事。
    //先頭の桁が１あがる形で丸まっていたら成功。
    //例　max 0.023　→　0.03    max -0.023 → -0.02
    if(max == 0){
        //なにもしない。
    }else if(max >= 1){
        iLog = (int)log10(max);
        iPow = qPow(10,iLog);
        max = max/iPow;
        max = ceil(max);
        max = max*iPow;
    }else if(max > 0){
        //ここはどうしようもない丸め誤差が出る。動作に問題はないが見た目が気持ち悪い。
        //例： 0.023 * 100 = 2.29999....
        //あるならば別途ライブラリを使うなどして対応したい所。
        iLog = (int)log10(max)-1;
        iPow = qPow(10,fabs(iLog));
        max = max*iPow;
        max = ceil(max);
        max = max/iPow;
    }else if(max > -1){
        iLog = (int)log10(fabs(max))-1;
        iPow = qPow(10,fabs(iLog));
        max = max/iPow;
        max = ceil(max);
        max = max*iPow*-1;
    }else {
        iLog = (int)log10(fabs(max));
        iPow = qPow(10,iLog);
        max = max/iPow*-1;
        max = ceil(max);
        max = max*iPow*-1;
    }

    //x軸最小値の算出
    if(min == 0){
        //なにもしない。
    }else if(min >= 1){
        iLog = (int)log10(min);
        iPow = qPow(10,iLog);
        min = min/iPow;
        min = floor(min);
        min = min*iPow;
    }else if(min > 0){
        iLog = (int)log10(min);
        iPow = qPow(10,fabs(iLog));
        min = min*iPow;
        min = floor(min);
        min = min/iPow;
    }else if(min > -1){
        iLog = (int)log10(fabs(min));
        iPow = qPow(10,fabs(iLog));
        min = min*iPow;
        min = floor(min);
        min = min/iPow;
    }else {
        iLog = (int)log10(fabs(min));
        iPow = qPow(10,iLog);
        min = min/iPow;
        min = floor(min);
        min = min*iPow;
    }

    //分割幅の計算
    double area = max - min;
    int tick = 10;
    double areaCount = area/tick;
    area = min;

    //カテゴリーの生成
    QStringList categories;
    for (int i = 0 ; i < tick ; i++){
        QString str;
        str = QString(QString::number(area));
        str += "～";
        area += areaCount;
        categories << str;
    }

    area = min + areaCount;
    int count = 0;
    QList <double> categorizeData;
    categorizeData.append(0);
    for (int i = 0 ; i < dataList.count() ; i++){
        if(dataList[i] < area){
            categorizeData[count] += 1;
        }else{
            count++;
            area += areaCount;
            categorizeData.append(0);
        }
    }


    //
    QBarSet *set0 = new QBarSet("histogram");
    for (int i = 0; i < categorizeData.count(); i++){
         *set0 << categorizeData[i] ;
    }

    //後はグラフィックビュー周りの設定
    QBarSeries *series = new QBarSeries();
    series->setBarWidth(1);
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram example");
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(420, 300);
    window.show();

    return a.exec();
}


