# Definition of LAT Pointing & Livetime History File (FT2)
# $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/data/ft2.tpl,v 1.10 2006/07/24 20:07:23 jchiang Exp $
SIMPLE      = T                           / file does conform to FITS standard
BITPIX      = 8                           / number of bits per data pixel
NAXIS       = 0                           / number of data axes
EXTEND      = T                           / FITS dataset may contain extensions
CHECKSUM    =                             / checksum for entire HDU
TELESCOP    = 'GLAST'                     / name of telescope generating data
INSTRUME    = 'LAT'                       / name of instrument generating data
EQUINOX     = 2000.0                      / equinox for ra and dec
RADECSYS    = 'FK5'                       / world coord. system for this file (FK5 or FK4)
DATE        =                             / file creation date (YYYY-MM-DDThh:mm:ss UT)
DATE-OBS    =                             / start date and time of the observation (UTC)
DATE-END    =                             / end date and time of the observation (UTC)
TSTART      =                                / mission time of the start of the observation
TSTOP       =                                / mission time of the end of the observation
MJDREFI     = 51910.                         / Integer part of MJD corresponding to SC clock start
MJDREFF     = 7.428703703703703D-4           / Fractional part of MJD corresponding to SC clock start
OBSERVER    = 'Peter Michelson'              / GLAST/LAT PI
FILENAME    =                             / name of this file
ORIGIN      = 'LISOC'                     / name of organization making file
#AUTHOR      =                             / name of person responsible for file generation
CREATOR     =                             / software and version creating file
VERSION     = 1                           / release version of the file
#SOFTWARE    =                             / version of the processing software
END

XTENSION     = 'BINTABLE'                  / binary table extension
BITPIX       = 8                           / 8-bit bytes
NAXIS        = 2                           / 2-dimensional binary table
NAXIS1       =                             / width of table in bytes
NAXIS2       =                             / number of rows in table
PCOUNT       =                             / size of special data area
GCOUNT       = 1                           / one data group (required keyword)
TFIELDS      =                             / number of fields in each row
CHECKSUM     =                             / checksum for entire HDU
DATASUM      =                             / checksum for data table
TELESCOP     = 'GLAST'                     / name of telescope generating data
INSTRUME     = 'LAT'                       / name of instrument generating data
EQUINOX      = 2000.0                      / equinox for ra and dec
RADECSYS     = 'FK5'                       / world coord. system for this file (FK5 or FK4)
DATE         =                             / file creation date (YYYY-MM-DDThh:mm:ss UT)
DATE-OBS     =                             / start date and time of the observation (UTC)
DATE-END     =                             / end date and time of the observation (UTC)
EXTNAME      = 'SC_DATA'                   / name of this binary table extension
TSTART       =                             / mission time of the start of the observation
TSTOP        =                             / mission time of the end of the observation
MJDREFI      = 51910.                      / Integer part of MJD corresponding to SC clock start
MJDREFF      = 7.428703703703703D-4        / Fractional part of MJD corresponding to SC clock start
TIMEUNIT     = 's'                         / units for the time related keywords
TIMEZERO     = 0.0                         / clock correction
TIMESYS      = 'TT'                        / type of time system that is used
TIMEREF      = 'LOCAL'                     / reference frame used for times
TASSIGN      = 'SATELLITE'                 / location where time assignment performed
CLOCKAPP     = F                           / whether a clock drift correction has been applied
GPS_OUT      = F                           / whether GPS time was unavailable at any time during this interval
TTYPE1       = 'START'                     / starting time of interval (Mission Elapsed Time)
TFORM1       = 'D'                         / data format of field: 8-byte DOUBLE
TUNIT1       = 's'                         / physical unit of field
TLMIN1       = 0.0                         / minimum value
TLMAX1       = 1.0D+10                     / maximum value
TTYPE2       = 'STOP'                      / ending time of interval (Mission Elapsed Time)
TFORM2       = 'D'                         / data format of field: 8-byte DOUBLE
TUNIT2       = 's'                         / physical unit of field
TLMIN2       = 0.0                         / minimum value
TLMAX2       = 1.0D+10                     / maximum value
TTYPE3       = 'SC_POSITION'               / S/C position at start of interval (x,y,z inertial coordinates)
TFORM3       = '3E'                        / data format of field: 4-byte REAL
TUNIT3       = 'm'                         / physical unit of field
TTYPE4       = 'LAT_GEO'                   / ground point latitude
TFORM4       = 'E'                         / data format of field: 4-byte REAL
TUNIT4       = 'deg'                       / physical unit of field
TLMIN4       = -90.0                       / minimum value
TLMAX4       = 90.0                        / maximum value
TTYPE5       = 'LON_GEO'                   / ground point longitude
TFORM5       = 'E'                         / data format of field: 4-byte REAL
TUNIT5       = 'deg'                       / physical unit of field
TLMIN5       = 0.0                         / minimum value
TLMAX5       = 360.0                       / maximum value
TTYPE6       = 'RAD_GEO'                   / S/C altitude
TFORM6       = 'D'                         / data format of field: 8-byte DOUBLE
TUNIT6       = 'm'                         / physical unit of field
TLMIN6       = 0.0                         / minimum value
TLMAX6       = 10000.0                     / maximum value
TTYPE7       = 'RA_ZENITH'                 / RA of zenith direction at start
TFORM7       = 'E'                         / data format of field: 8-byte DOUBLE
TUNIT7       = 'deg'                       / physical unit of field: dimensionless
TLMIN7       = 0.0                         / minimum value
TLMAX7       = 360.0                       / maximum value
TTYPE8       = 'DEC_ZENITH'                / Dec of zenith direction at start
TFORM8       = 'E'                         / data format of field: 4-byte REAL
TUNIT8       = 'deg'                       / physical unit of field: dimensionless
TLMIN8       = -90.0                       / minimum value
TLMAX8       = 90.0                        / maximum value
TTYPE9       = 'B_MCILWAIN'                / McIlwain B parameter, magnetic field
TFORM9       = 'E'                         / data format of field: 4-byte REAL
TUNIT9       = 'Gauss'                     / physical unit of field
TLMIN9       = 0.0                         / minimum value
TLMAX9       = 100.0                       / maximum value
TTYPE10      = 'L_MCILWAIN'                / McIlwain L parameter, distance
TFORM10      = 'E'                         / data format of field: 4-byte REAL
TUNIT10      = 'Earth_Radii'               / physical unit of field
TLMIN10      = 0.0                         / minimum value
TLMAX10      = 100.0                       / maximum value
TTYPE11      = 'IN_SAA'                    / whether spacecraft was in SAA
TFORM11      = 'L'                         / data format of field: logical
TTYPE12      = 'RA_SCZ'                    / viewing direction at start (RA of LAT +z axis)
TFORM12      = 'E'                         / data format of field: 8-byte DOUBLE
TUNIT12      = 'deg'                       / physical unit of field: dimensionless
TLMIN12      = 0.0                         / minimum value
TLMAX12      = 360.0                       / maximum value
TTYPE13      = 'DEC_SCZ'                   / viewing direction at start (Dec of LAT +z axis)
TFORM13      = 'E'                         / data format of field: 4-byte REAL
TUNIT13      = 'deg'                       / physical unit of field: dimensionless
TLMIN13      = -90.0                       / minimum value
TLMAX13      = 90.0                        / maximum value
TTYPE14      = 'RA_SCX'                    / viewing direction at start (RA of LAT +x axis)
TFORM14      = 'E'                         / data format of field: 8-byte DOUBLE
TUNIT14      = 'deg'                       / physical unit of field: dimensionless
TLMIN14      = 0.0                         / minimum value
TLMAX14      = 360.0                       / maximum value
TTYPE15      = 'DEC_SCX'                   / viewing direction at start (Dec of LAT +x axis)
TFORM15      = 'E'                         / data format of field: 4-byte REAL
TUNIT15      = 'deg'                       / physical unit of field: dimensionless
TLMIN15      = -90.0                       / minimum value
TLMAX15      = 90.0                        / maximum value
TTYPE16      = 'LAT_MODE'                  / operation mode of LAT
TFORM16      = 'J'                         / data format of field: 4-byte signed INTEGER
TLMIN16      = 0                           / minimum value
TLMAX16      = 2147483647                  / maximum value
TTYPE17      = 'LIVETIME'                  / livetime
TFORM17      = 'D'                         / data format of field: 8-byte DOUBLE
TUNIT17      = 's'                         / physical unit of field
TLMIN17      = 0.0                         / minimum value
TLMAX17      = 1.0D+10                     / maximum value
TTYPE18      = 'DEADTIME'                  / deadtime accumulated since start of mission
TFORM18      = 'D'                         / data format of field: 8-byte DOUBLE
TUNIT18      = 's'                         / physical unit of field
TLMIN18      = 0.0                         / minimum value
TLMAX18      = 1.0D+10                     / maximum value
END
