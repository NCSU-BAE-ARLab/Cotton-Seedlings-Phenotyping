#include <iostream>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>

int main()
{
   pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

   // Read cloud data
   pcl::PLYReader reader;
   reader.read<pcl::PointXYZRGB> ("path/to/ply/file", *cloud); // Replace path with PLY file

   // Set conditions for ConditionalRemoval
   // x
   pcl::ConditionAnd<pcl::PointXYZRGB>::Ptr xCond (new pcl::ConditionAnd<pcl::PointXYZRGB> ());
   xCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("x", pcl::ComparisonOps::GE, -0.33)));
   xCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("x", pcl::ComparisonOps::LE, 0.37)));

   // y
   pcl::ConditionAnd<pcl::PointXYZRGB>::Ptr yCond (new pcl::ConditionAnd<pcl::PointXYZRGB> ());
   yCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("y", pcl::ComparisonOps::GE, -0.28)));
   yCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("y", pcl::ComparisonOps::LE, 0.25)));

   // z
   pcl::ConditionAnd<pcl::PointXYZRGB>::Ptr zCond (new pcl::ConditionAnd<pcl::PointXYZRGB> ());
   zCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("z", pcl::ComparisonOps::GE, 0.37)));
   zCond->addComparison (pcl::FieldComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::FieldComparison<pcl::PointXYZRGB> ("z", pcl::ComparisonOps::LE, 0.61)));

   // g
   pcl::ConditionAnd<pcl::PointXYZRGB>::Ptr gCond (new pcl::ConditionAnd<pcl::PointXYZRGB> ());
   gCond->addComparison (pcl::PackedRGBComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::PackedRGBComparison<pcl::PointXYZRGB> ("g", pcl::ComparisonOps::GE, 60)));

   // No white color
   pcl::ConditionOr<pcl::PointXYZRGB>::Ptr noWhiteCond (new pcl::ConditionOr<pcl::PointXYZRGB> ());
    noWhiteCond->addComparison (pcl::PackedRGBComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::PackedRGBComparison<pcl::PointXYZRGB> ("r", pcl::ComparisonOps::LE, 250)));
    noWhiteCond->addComparison (pcl::PackedRGBComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::PackedRGBComparison<pcl::PointXYZRGB> ("g", pcl::ComparisonOps::LE, 250)));
    noWhiteCond->addComparison (pcl::PackedRGBComparison<pcl::PointXYZRGB>::ConstPtr (new
      pcl::PackedRGBComparison<pcl::PointXYZRGB> ("b", pcl::ComparisonOps::LE, 250)));

   // Overall condition object
    pcl::ConditionAnd<pcl::PointXYZRGB>::Ptr cond (new pcl::ConditionAnd<pcl::PointXYZRGB> ());
    cond->addCondition(xCond);
    cond->addCondition(yCond);
    cond->addCondition(zCond);
    cond->addCondition(gCond);
    cond->addCondition(noWhiteCond);

    // ConditionalRemoval filter
    pcl::ConditionalRemoval<pcl::PointXYZRGB> cr;
    cr.setCondition (cond);
    cr.setInputCloud (cloud);
    cr.setKeepOrganized(false);
    cr.filter (*cloud);

   // RadiusOutlierRemoval filter
   pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> ror;
   ror.setInputCloud(cloud);
   ror.setRadiusSearch(0.005);
   ror.setMinNeighborsInRadius (100);
   ror.setKeepOrganized(false);
   ror.filter (*cloud);

   // StatisticalOutlierRemoval filter
   pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
   sor.setInputCloud (cloud);
   sor.setMeanK (50);
   sor.setStddevMulThresh (1.7);
   sor.filter (*cloud);

   // Write filtered cloud
   pcl::PLYWriter writer;
   writer.write<pcl::PointXYZRGB> ("filtered.ply", *cloud, false);

   return (0);
}